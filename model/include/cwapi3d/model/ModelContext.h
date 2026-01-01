#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "cwapi3d/model/Building.h"
#include "cwapi3d/model/BuildingAssignment.h"
#include "cwapi3d/model/BuildingStorey.h"
#include "cwapi3d/model/BuildingStoreyAssignment.h"
#include "cwapi3d/model/ComponentRegistry.h"
#include "cwapi3d/model/Element.h"
#include "cwapi3d/model/ElementAssignment.h"
#include "cwapi3d/model/EntityId.h"
#include "cwapi3d/model/cwapi3d_model_export.h"

namespace CwAPI3D::Model {

/**
 * @brief Root of all model state - the "Document" or "Session" context
 * 
 * ModelContext is the single source of truth for a CAD/BIM model session.
 * It owns all domain entities, registries, and relations.
 * 
 * Design principles:
 * - No global state: All state lives here
 * - No singletons: Create multiple contexts for multi-document scenarios
 * - Clear ownership: Context owns entities, registries own components
 * - Service layer accesses context, not vice versa
 * 
 * Responsibilities:
 * - Entity lifecycle management (create, delete)
 * - Component registry ownership
 * - Relation registry ownership
 * - Query support for domain entities
 * 
 * Usage:
 *   ModelContext context;
 *   auto* building = context.createBuilding("My Building");
 *   auto* floor = context.createFloor("Ground Floor", 0.0);
 *   auto* element = context.createElement("Wall");
 */
class CWAPI3D_MODEL_EXPORT ModelContext {
   public:
    ModelContext() = default;
    ~ModelContext() = default;

    // Non-copyable, movable
    ModelContext(const ModelContext&) = delete;
    ModelContext& operator=(const ModelContext&) = delete;
    ModelContext(ModelContext&&) = default;
    ModelContext& operator=(ModelContext&&) = default;

    // ========== Entity Creation ==========

    /**
     * @brief Create a new building
     * Returns a non-owning pointer (context owns the entity)
     */
    Building* createBuilding(std::string name = "Building");

    /**
     * @brief Create a new floor
     */
    BuildingStorey* createFloor(std::string name = "Floor",
                       std::optional<double> elevation = std::nullopt);

    /**
     * @brief Create a new element
     */
    Element* createElement(std::string name = "Element");

    // ========== Entity Lookup ==========

    [[nodiscard]] Building* getBuilding(const EntityId& id) const;
    [[nodiscard]] BuildingStorey* getFloor(const EntityId& id) const;
    [[nodiscard]] Element* getElement(const EntityId& id) const;

    [[nodiscard]] std::vector<Building*> getAllBuildings() const;
    [[nodiscard]] std::vector<BuildingStorey*> getAllFloors() const;
    [[nodiscard]] std::vector<Element*> getAllElements() const;

    // ========== Entity Deletion ==========

    bool deleteBuilding(const EntityId& id);
    bool deleteFloor(const EntityId& id);
    bool deleteElement(const EntityId& id);

    // ========== Registry Access ==========

    [[nodiscard]] ComponentRegistry& componentRegistry() noexcept { return mComponentRegistry; }
    [[nodiscard]] const ComponentRegistry& componentRegistry() const noexcept {
        return mComponentRegistry;
    }

    [[nodiscard]] BuildingStoreyAssignment& floorAssignment() noexcept { return mFloorAssignment; }
    [[nodiscard]] const BuildingStoreyAssignment& floorAssignment() const noexcept {
        return mFloorAssignment;
    }

    [[nodiscard]] BuildingAssignment& buildingAssignment() noexcept {
        return mBuildingAssignment;
    }
    [[nodiscard]] const BuildingAssignment& buildingAssignment() const noexcept {
        return mBuildingAssignment;
    }

    [[nodiscard]] ElementAssignment& elementAssignment() noexcept {
        return mElementAssignment;
    }
    [[nodiscard]] const ElementAssignment& elementAssignment() const noexcept {
        return mElementAssignment;
    }

    // ========== Statistics ==========

    [[nodiscard]] size_t buildingCount() const noexcept { return mBuildings.size(); }
    [[nodiscard]] size_t floorCount() const noexcept { return mFloors.size(); }
    [[nodiscard]] size_t elementCount() const noexcept { return mElements.size(); }

    /**
     * @brief Clear entire model (for new document or reset)
     */
    void clear();

   private:
    // Entity storage (owns entities)
    std::unordered_map<EntityId, std::unique_ptr<Building>> mBuildings;
    std::unordered_map<EntityId, std::unique_ptr<BuildingStorey>> mFloors;
    std::unordered_map<EntityId, std::unique_ptr<Element>> mElements;

    // Registries (owns components and relations)
    ComponentRegistry mComponentRegistry;
    BuildingStoreyAssignment mFloorAssignment;
    BuildingAssignment mBuildingAssignment;
    ElementAssignment mElementAssignment;
};

}  // namespace CwAPI3D::Model
