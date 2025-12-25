#include "cwapi3d/model/BuildingAssignment.h"

namespace CwAPI3D::Model {

void BuildingAssignment::assignFloorToBuilding(const EntityId& floorId,
                                                const EntityId& buildingId) {
    auto prevBuildingIt = mFloorToBuilding.find(floorId);
    if (prevBuildingIt != mFloorToBuilding.end()) {
        auto& prevFloors = mBuildingToFloors[prevBuildingIt->second];
        prevFloors.erase(floorId);

        if (prevFloors.empty()) {
            mBuildingToFloors.erase(prevBuildingIt->second);
        }
    }

    mFloorToBuilding[floorId] = buildingId;
    mBuildingToFloors[buildingId].insert(floorId);
}

bool BuildingAssignment::removeFloorFromBuilding(const EntityId& floorId) {
    auto it = mFloorToBuilding.find(floorId);
    if (it == mFloorToBuilding.end()) {
        return false;
    }

    const EntityId buildingId = it->second;
    mFloorToBuilding.erase(it);

    auto buildingIt = mBuildingToFloors.find(buildingId);
    if (buildingIt != mBuildingToFloors.end()) {
        buildingIt->second.erase(floorId);

        if (buildingIt->second.empty()) {
            mBuildingToFloors.erase(buildingIt);
        }
    }

    return true;
}

std::optional<EntityId> BuildingAssignment::getBuildingForFloor(const EntityId& floorId) const {
    auto it = mFloorToBuilding.find(floorId);
    if (it == mFloorToBuilding.end()) {
        return std::nullopt;
    }
    return it->second;
}

bool BuildingAssignment::isFloorInBuilding(const EntityId& floorId,
                                            const EntityId& buildingId) const {
    auto it = mFloorToBuilding.find(floorId);
    return it != mFloorToBuilding.end() && it->second == buildingId;
}

std::vector<EntityId> BuildingAssignment::getFloorsInBuilding(const EntityId& buildingId) const {
    auto it = mBuildingToFloors.find(buildingId);
    if (it == mBuildingToFloors.end()) {
        return {};
    }

    return std::vector<EntityId>(it->second.begin(), it->second.end());
}

size_t BuildingAssignment::getFloorCountInBuilding(const EntityId& buildingId) const {
    auto it = mBuildingToFloors.find(buildingId);
    if (it == mBuildingToFloors.end()) {
        return 0;
    }
    return it->second.size();
}

void BuildingAssignment::removeBuilding(const EntityId& buildingId) {
    auto buildingIt = mBuildingToFloors.find(buildingId);
    if (buildingIt == mBuildingToFloors.end()) {
        return;
    }

    for (const auto& floorId : buildingIt->second) {
        mFloorToBuilding.erase(floorId);
    }

    mBuildingToFloors.erase(buildingIt);
}

std::vector<EntityId> BuildingAssignment::getBuildingsWithFloors() const {
    std::vector<EntityId> result;
    result.reserve(mBuildingToFloors.size());

    for (const auto& [buildingId, _] : mBuildingToFloors) {
        result.push_back(buildingId);
    }

    return result;
}

void BuildingAssignment::clear() {
    mFloorToBuilding.clear();
    mBuildingToFloors.clear();
}

}  // namespace CwAPI3D::Model
