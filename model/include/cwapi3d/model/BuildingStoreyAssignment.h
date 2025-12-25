#pragma once

#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "cwapi3d/model/EntityId.h"
#include "cwapi3d/model/cwapi3d_model_export.h"

namespace CwAPI3D::Model {

/**
 * @brief Manages element-to-floor spatial assignment relationships
 * 
 * This is NOT a containment hierarchy. It's a relation registry that allows:
 * - Runtime reassignment without restructuring objects
 * - Query in both directions (element -> floor, floor -> elements)
 * - Undo/Redo friendly operations
 * - Multi-floor assignment support (optional)
 * 
 * Design principles:
 * - Relations are separate from domain entities
 * - No bidirectional links in objects themselves
 * - All relation logic centralized here
 * - Supports single-assignment (one element, one floor)
 * 
 * Example:
 *   relations.assignElementToFloor(element.id(), floor.id());
 *   auto floorId = relations.getFloorForElement(element.id());
 *   auto elements = relations.getElementsOnFloor(floor.id());
 */
class CWAPI3D_MODEL_EXPORT BuildingStoreyAssignment {
   public:
    BuildingStoreyAssignment() = default;
    ~BuildingStoreyAssignment() = default;

    // Non-copyable, movable
    BuildingStoreyAssignment(const BuildingStoreyAssignment&) = delete;
    BuildingStoreyAssignment& operator=(const BuildingStoreyAssignment&) = delete;
    BuildingStoreyAssignment(BuildingStoreyAssignment&&) = default;
    BuildingStoreyAssignment& operator=(BuildingStoreyAssignment&&) = default;

    /**
     * @brief Assign an element to a floor
     * 
     * If the element is already assigned, it will be reassigned to the new floor.
     */
    void assignElementToFloor(const EntityId& elementId, const EntityId& floorId);

    /**
     * @brief Remove element from its assigned floor
     * 
     * Returns true if the element was assigned and has been removed.
     */
    bool removeElementFromFloor(const EntityId& elementId);

    /**
     * @brief Get the floor that an element is assigned to
     * 
     * Returns std::nullopt if the element is not assigned to any floor.
     */
    [[nodiscard]] std::optional<EntityId> getFloorForElement(const EntityId& elementId) const;

    /**
     * @brief Check if an element is assigned to a specific floor
     */
    [[nodiscard]] bool isElementOnFloor(const EntityId& elementId,
                                         const EntityId& floorId) const;

    /**
     * @brief Get all elements assigned to a specific floor
     */
    [[nodiscard]] std::vector<EntityId> getElementsOnFloor(const EntityId& floorId) const;

    /**
     * @brief Get count of elements on a floor
     */
    [[nodiscard]] size_t getElementCountOnFloor(const EntityId& floorId) const;

    /**
     * @brief Remove all assignments for a floor (when floor is deleted)
     */
    void removeFloor(const EntityId& floorId);

    /**
     * @brief Get all floors that have at least one element
     */
    [[nodiscard]] std::vector<EntityId> getFloorsWithElements() const;

    /**
     * @brief Clear all assignments
     */
    void clear();

   private:
    // Element -> Floor
    std::unordered_map<EntityId, EntityId> mElementToFloor;

    // Floor -> Set of Elements
    std::unordered_map<EntityId, std::unordered_set<EntityId>> mFloorToElements;
};

}  // namespace CwAPI3D::Model
