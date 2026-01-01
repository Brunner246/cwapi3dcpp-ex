#include "cwapi3d/model/ElementAssignment.h"

#include <algorithm>

namespace CwAPI3D::Model {

void ElementAssignment::assignElementToParent(const EntityId& childId, const EntityId& parentId) {
    // Prevent self-reference
    if (childId == parentId) {
        return;
    }

    // Remove from previous parent if exists
    if (const auto prevParentIt = mChildToParent.find(childId);
        prevParentIt != mChildToParent.end()) {
        const EntityId& prevParent = prevParentIt->second;
        auto& prevChildren = mParentToChildren[prevParent];
        prevChildren.erase(childId);
        if (prevChildren.empty()) {
            mParentToChildren.erase(prevParent);
        }
    }

    // Assign to new parent
    mChildToParent[childId] = parentId;
    mParentToChildren[parentId].insert(childId);
}

bool ElementAssignment::removeElementFromParent(const EntityId& childId) {
    const auto it = mChildToParent.find(childId);
    if (it == mChildToParent.end()) {
        return false;  // Not assigned
    }

    const EntityId& parentId = it->second;
    auto& children = mParentToChildren[parentId];
    children.erase(childId);

    if (children.empty()) {
        mParentToChildren.erase(parentId);
    }

    mChildToParent.erase(it);
    return true;
}

std::optional<EntityId> ElementAssignment::getParentForElement(const EntityId& childId) const {
    const auto it = mChildToParent.find(childId);
    if (it != mChildToParent.end()) {
        return it->second;
    }
    return std::nullopt;
}

bool ElementAssignment::isElementChildOf(const EntityId& childId,
                                          const EntityId& parentId) const {
    const auto it = mChildToParent.find(childId);
    return it != mChildToParent.end() && it->second == parentId;
}

std::vector<EntityId> ElementAssignment::getChildElements(const EntityId& parentId) const {
    const auto it = mParentToChildren.find(parentId);
    if (it == mParentToChildren.end()) {
        return {};
    }

    std::vector<EntityId> children;
    children.reserve(it->second.size());
    for (const auto& childId : it->second) {
        children.push_back(childId);
    }
    return children;
}

size_t ElementAssignment::getChildElementCount(const EntityId& parentId) const {
    const auto it = mParentToChildren.find(parentId);
    if (it == mParentToChildren.end()) {
        return 0;
    }
    return it->second.size();
}

void ElementAssignment::removeParentElement(const EntityId& parentId) {
    const auto it = mParentToChildren.find(parentId);
    if (it == mParentToChildren.end()) {
        return;
    }

    // Remove all child->parent mappings
    for (const auto& childId : it->second) {
        mChildToParent.erase(childId);
    }

    // Remove parent->children mapping
    mParentToChildren.erase(it);
}

std::vector<EntityId> ElementAssignment::getParentsWithChildren() const {
    std::vector<EntityId> parents;
    parents.reserve(mParentToChildren.size());

    for (const auto& [parentId, children] : mParentToChildren) {
        if (!children.empty()) {
            parents.push_back(parentId);
        }
    }

    return parents;
}

void ElementAssignment::clear() {
    mChildToParent.clear();
    mParentToChildren.clear();
}

}  // namespace CwAPI3D::Model
