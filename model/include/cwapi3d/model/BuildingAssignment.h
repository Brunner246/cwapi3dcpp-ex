#pragma once

#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "cwapi3d/model/EntityId.h"
#include "cwapi3d/model/cwapi3d_model_export.h"

namespace CwAPI3D::Model {

/**
 * @brief Manages floor-to-building assignment relationships
 * 
 * Similar to FloorAssignment, this manages the relation between floors and buildings.
 * Allows runtime reassignment and supports multi-building scenarios.
 */
class CWAPI3D_MODEL_EXPORT BuildingAssignment {
   public:
    BuildingAssignment() = default;
    ~BuildingAssignment() = default;

    BuildingAssignment(const BuildingAssignment&) = delete;
    BuildingAssignment& operator=(const BuildingAssignment&) = delete;
    BuildingAssignment(BuildingAssignment&&) = default;
    BuildingAssignment& operator=(BuildingAssignment&&) = default;

    void assignFloorToBuilding(const EntityId& floorId, const EntityId& buildingId);
    bool removeFloorFromBuilding(const EntityId& floorId);

    [[nodiscard]] std::optional<EntityId> getBuildingForFloor(const EntityId& floorId) const;
    [[nodiscard]] bool isFloorInBuilding(const EntityId& floorId,
                                          const EntityId& buildingId) const;
    [[nodiscard]] std::vector<EntityId> getFloorsInBuilding(const EntityId& buildingId) const;
    [[nodiscard]] size_t getFloorCountInBuilding(const EntityId& buildingId) const;

    void removeBuilding(const EntityId& buildingId);
    [[nodiscard]] std::vector<EntityId> getBuildingsWithFloors() const;

    void clear();

   private:
    std::unordered_map<EntityId, EntityId> mFloorToBuilding;
    std::unordered_map<EntityId, std::unordered_set<EntityId>> mBuildingToFloors;
};

}  // namespace CwAPI3D::Model
