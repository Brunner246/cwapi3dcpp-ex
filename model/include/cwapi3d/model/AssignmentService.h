#pragma once

#include <memory>
#include <vector>

#include "cwapi3d/model/EntityId.h"
#include "cwapi3d/model/IComponent.h"
#include "cwapi3d/model/ModelContext.h"
#include "cwapi3d/model/cwapi3d_model_export.h"

namespace CwAPI3D::Model {

/**
 * @brief Service layer for assignment operations
 * 
 * All mutation logic lives in services, not in domain entities.
 * This follows the Domain-Driven Design principle of keeping entities simple
 * and putting complex business logic in services.
 * 
 * Benefits:
 * - Clear separation of concerns
 * - Testable without complex mocking
 * - Easy to add validation, logging, events
 * - Supports command pattern for Undo/Redo
 * 
 * Services operate on ModelContext, never directly on entities.
 */
class CWAPI3D_MODEL_EXPORT AssignmentService {
   public:
    struct FloorBucket {
        BuildingStorey* floor{nullptr};
        std::vector<Element*> elements;
    };

    struct BuildingBucket {
        Building* building{nullptr};
        std::vector<FloorBucket> floors;
    };

    explicit AssignmentService(ModelContext& context) : mContext(context) {}

    // ========== Floor Assignment ==========

    /**
     * @brief Assign an element to a floor
     * 
     * Validates that both entities exist before performing the assignment.
     * Returns true if successful, false if validation fails.
     */
    [[nodiscard]] bool assignElementToFloor(const EntityId& elementId, const EntityId& floorId) const;

    /**
     * @brief Remove element from its assigned floor
     */
    [[nodiscard]] bool removeElementFromFloor(const EntityId& elementId) const;

    /**
     * @brief Get all elements on a specific floor
     * 
     * Returns actual Element pointers, not just IDs.
     */
    [[nodiscard]] std::vector<Element*> getElementsOnFloor(const EntityId& floorId) const;

    // ========== Building Assignment ==========

    /**
     * @brief Assign a floor to a building
     */
    [[nodiscard]] bool assignFloorToBuilding(const EntityId& floorId, const EntityId& buildingId) const;

    /**
     * @brief Remove floor from its assigned building
     */
    [[nodiscard]] bool removeFloorFromBuilding(const EntityId& floorId) const;

    /**
     * @brief Get all floors in a specific building
     */
    [[nodiscard]] std::vector<BuildingStorey*> getFloorsInBuilding(const EntityId& buildingId) const;

    // ========== Component Management ==========

    /**
     * @brief Add a component to an entity (element, floor, or building)
     * 
     * Validates that the entity exists before adding the component.
     */
    template <typename T>
    bool addComponent(const EntityId& entityId, std::unique_ptr<T> component) {
        // Validate entity exists (check all entity types)
        if (!entityExists(entityId)) {
            return false;
        }

        mContext.componentRegistry().addComponent(entityId, std::move(component));
        return true;
    }

    /**
     * @brief Remove a component from an entity
     */
    template <typename T>
    [[nodiscard]] bool removeComponent(const EntityId& entityId) const {
        return mContext.componentRegistry().removeComponent<T>(entityId);
    }

    /**
     * @brief Get a component from an entity
     */
    template <typename T>
    [[nodiscard]] T* getComponent(const EntityId& entityId) const {
        return mContext.componentRegistry().getComponent<T>(entityId);
    }

    // ========== Hierarchical Queries ==========

    /**
     * @brief Get all elements in a building (across all floors)
     */
    [[nodiscard]] std::vector<Element*> getElementsInBuilding(const EntityId& buildingId) const;

    /**
     * @brief Get the building that contains an element (through floor relation)
     */
    [[nodiscard]] Building* getBuildingForElement(const EntityId& elementId) const;

    /**
     * @brief Build a hierarchical snapshot of buildings -> floors -> elements
     *
     * Useful for APIs that need ordered structures without mutating the model.
     */
    [[nodiscard]] std::vector<BuildingBucket> getHierarchy() const;

   private:
    [[nodiscard]] bool entityExists(const EntityId& entityId) const;

    ModelContext& mContext;
};

}  // namespace CwAPI3D::Model
