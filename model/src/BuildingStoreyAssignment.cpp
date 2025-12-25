#include "cwapi3d/model/BuildingStoreyAssignment.h"

namespace CwAPI3D::Model {

void BuildingStoreyAssignment::assignElementToFloor(const EntityId& elementId, const EntityId& floorId) {
    // Remove from previous floor if assigned
    auto prevFloorIt = mElementToFloor.find(elementId);
    if (prevFloorIt != mElementToFloor.end()) {
        auto& prevElements = mFloorToElements[prevFloorIt->second];
        prevElements.erase(elementId);

        // Clean up empty floor entry
        if (prevElements.empty()) {
            mFloorToElements.erase(prevFloorIt->second);
        }
    }

    // Assign to new floor
    mElementToFloor[elementId] = floorId;
    mFloorToElements[floorId].insert(elementId);
}

bool BuildingStoreyAssignment::removeElementFromFloor(const EntityId& elementId) {
    auto it = mElementToFloor.find(elementId);
    if (it == mElementToFloor.end()) {
        return false;
    }

    const EntityId floorId = it->second;
    mElementToFloor.erase(it);

    auto floorIt = mFloorToElements.find(floorId);
    if (floorIt != mFloorToElements.end()) {
        floorIt->second.erase(elementId);

        // Clean up empty floor entry
        if (floorIt->second.empty()) {
            mFloorToElements.erase(floorIt);
        }
    }

    return true;
}

std::optional<EntityId> BuildingStoreyAssignment::getFloorForElement(const EntityId& elementId) const {
    auto it = mElementToFloor.find(elementId);
    if (it == mElementToFloor.end()) {
        return std::nullopt;
    }
    return it->second;
}

bool BuildingStoreyAssignment::isElementOnFloor(const EntityId& elementId, const EntityId& floorId) const {
    auto it = mElementToFloor.find(elementId);
    return it != mElementToFloor.end() && it->second == floorId;
}

std::vector<EntityId> BuildingStoreyAssignment::getElementsOnFloor(const EntityId& floorId) const {
    auto it = mFloorToElements.find(floorId);
    if (it == mFloorToElements.end()) {
        return {};
    }

    return {it->second.begin(), it->second.end()};
}

size_t BuildingStoreyAssignment::getElementCountOnFloor(const EntityId& floorId) const {
    auto it = mFloorToElements.find(floorId);
    if (it == mFloorToElements.end()) {
        return 0;
    }
    return it->second.size();
}

void BuildingStoreyAssignment::removeFloor(const EntityId& floorId) {
    auto floorIt = mFloorToElements.find(floorId);
    if (floorIt == mFloorToElements.end()) {
        return;
    }

    // Remove all element mappings to this floor
    for (const auto& elementId : floorIt->second) {
        mElementToFloor.erase(elementId);
    }

    // Remove the floor entry
    mFloorToElements.erase(floorIt);
}

std::vector<EntityId> BuildingStoreyAssignment::getFloorsWithElements() const {
    std::vector<EntityId> result;
    result.reserve(mFloorToElements.size());

    for (const auto& [floorId, _] : mFloorToElements) {
        result.push_back(floorId);
    }

    return result;
}

void BuildingStoreyAssignment::clear() {
    mElementToFloor.clear();
    mFloorToElements.clear();
}

}  // namespace CwAPI3D::Model
