#pragma once

#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "cwapi3d/model/EntityId.h"
#include "cwapi3d/model/cwapi3d_model_export.h"

namespace CwAPI3D::Model {

/**
 * @brief Manages element-to-parent-element hierarchical relationships
 * 
 * This enables recursive aggregation where an Element can contain sub-elements.
 * For example: A Wall (Element) can contain Posts and Plates (sub-elements).
 * 
 * Design principles:
 * - Relations are separate from domain entities
 * - No bidirectional links in objects themselves
 * - All relation logic centralized here
 * - Supports single-parent assignment (one child element, one parent element)
 * 
 * Example:
 *   relations.assignElementToParent(postId, wallId);
 *   auto parentId = relations.getParentForElement(postId);
 *   auto children = relations.getChildElements(wallId);
 */
class CWAPI3D_MODEL_EXPORT ElementAssignment {
   public:
    ElementAssignment() = default;
    ~ElementAssignment() = default;

    // Non-copyable, movable
    ElementAssignment(const ElementAssignment&) = delete;
    ElementAssignment& operator=(const ElementAssignment&) = delete;
    ElementAssignment(ElementAssignment&&)  noexcept = default;
    ElementAssignment& operator=(ElementAssignment&&) = default;

    /**
     * @brief Assign a child element to a parent element
     * 
     * If the child was previously assigned to another parent, it will be reassigned.
     * An element cannot be its own parent (self-reference check).
     */
    void assignElementToParent(const EntityId& childId, const EntityId& parentId);

    /**
     * @brief Remove a child element from its parent
     * 
     * Returns true if the element was removed, false if it wasn't assigned.
     */
    bool removeElementFromParent(const EntityId& childId);

    /**
     * @brief Get the parent element for a given child element
     */
    [[nodiscard]] std::optional<EntityId> getParentForElement(const EntityId& childId) const;

    /**
     * @brief Check if an element is a child of a specific parent
     */
    [[nodiscard]] bool isElementChildOf(const EntityId& childId,
                                         const EntityId& parentId) const;

    /**
     * @brief Get all child elements for a given parent
     */
    [[nodiscard]] std::vector<EntityId> getChildElements(const EntityId& parentId) const;

    /**
     * @brief Get the number of child elements for a given parent
     */
    [[nodiscard]] size_t getChildElementCount(const EntityId& parentId) const;

    /**
     * @brief Remove all children from a parent element
     * 
     * This is useful when deleting an element - it removes all child assignments.
     */
    void removeParentElement(const EntityId& parentId);

    /**
     * @brief Get all parent elements that have children
     */
    [[nodiscard]] std::vector<EntityId> getParentsWithChildren() const;

    /**
     * @brief Clear all element-to-parent assignments
     */
    void clear();

   private:
    // Child -> Parent mapping
    std::unordered_map<EntityId, EntityId> mChildToParent;

    // Parent -> Children mapping
    std::unordered_map<EntityId, std::unordered_set<EntityId>> mParentToChildren;
};

}  // namespace CwAPI3D::Model
