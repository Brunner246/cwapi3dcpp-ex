#include "cwapi3d/model/AssignmentService.h"
#include "cwapi3d/model/Element.h"


namespace CwAPI3D::Model {

// ========== Floor Assignment ==========

bool AssignmentService::assignElementToFloor(const EntityId& elementId, const EntityId& floorId) const {
    // Validate entities exist
    if (!mContext.getElement(elementId) || !mContext.getFloor(floorId)) {
        return false;
    }

    mContext.floorAssignment().assignElementToFloor(elementId, floorId);
    return true;
}

bool AssignmentService::removeElementFromFloor(const EntityId& elementId) const {
    return mContext.floorAssignment().removeElementFromFloor(elementId);
}

std::vector<Element*> AssignmentService::getElementsOnFloor(const EntityId& floorId) const {
    const auto elementIds = mContext.floorAssignment().getElementsOnFloor(floorId);

    std::vector<Element*> result;
    result.reserve(elementIds.size());

    for (const auto& id : elementIds) {
        if (auto* element = mContext.getElement(id)) {
            result.push_back(element);
        }
    }

    return result;
}

// ========== Building Assignment ==========

bool AssignmentService::assignFloorToBuilding(const EntityId& floorId,
                                               const EntityId& buildingId) const {
    if (!mContext.getFloor(floorId) || !mContext.getBuilding(buildingId)) {
        return false;
    }

    mContext.buildingAssignment().assignFloorToBuilding(floorId, buildingId);
    return true;
}

bool AssignmentService::removeFloorFromBuilding(const EntityId& floorId) const {
    return mContext.buildingAssignment().removeFloorFromBuilding(floorId);
}

std::vector<BuildingStorey*> AssignmentService::getFloorsInBuilding(const EntityId& buildingId) const {
    const auto floorIds = mContext.buildingAssignment().getFloorsInBuilding(buildingId);

    std::vector<BuildingStorey*> result;
    result.reserve(floorIds.size());

    for (const auto& id : floorIds) {
        if (auto* floor = mContext.getFloor(id)) {
            result.push_back(floor);
        }
    }

    return result;
}

// ========== Hierarchical Queries ==========

std::vector<Element*> AssignmentService::getElementsInBuilding(const EntityId& buildingId) const {
    std::vector<Element*> result;

    // Get all floors in the building
    const auto floors = getFloorsInBuilding(buildingId);

    // Get all elements from each floor
    for (const auto* floor : floors) {
        const auto elements = getElementsOnFloor(floor->id());
        result.insert(result.end(), elements.begin(), elements.end());
    }

    return result;
}

Building* AssignmentService::getBuildingForElement(const EntityId& elementId) const {
    // Element -> Floor
    const auto floorId = mContext.floorAssignment().getFloorForElement(elementId);
    if (!floorId) {
        return nullptr;
    }

    // Floor -> Building
    const auto buildingId = mContext.buildingAssignment().getBuildingForFloor(*floorId);
    if (!buildingId) {
        return nullptr;
    }

    return mContext.getBuilding(*buildingId);
}

std::vector<AssignmentService::BuildingBucket> AssignmentService::getHierarchy() const {
    std::vector<BuildingBucket> result;
    const auto buildings = mContext.getAllBuildings();
    result.reserve(buildings.size());

    for (auto* building : buildings) {
        BuildingBucket buildingBucket{};
        buildingBucket.building = building;

        const auto floors = getFloorsInBuilding(building->id());
        buildingBucket.floors.reserve(floors.size());

        for (auto* floor : floors) {
            FloorBucket floorBucket{};
            floorBucket.floor = floor;
            floorBucket.elements = getElementsOnFloor(floor->id());
            buildingBucket.floors.push_back(std::move(floorBucket));
        }

        result.push_back(std::move(buildingBucket));
    }

    return result;
}

// ========== Private Helpers ==========

bool AssignmentService::entityExists(const EntityId& entityId) const {
    return mContext.getElement(entityId) || mContext.getFloor(entityId) ||
           mContext.getBuilding(entityId);
}

// ========== Element Hierarchy (Recursive Aggregation) ==========

bool AssignmentService::assignElementToParent(const EntityId& childId,
                                               const EntityId& parentId) const {
    // Validate entities exist
    if (!mContext.getElement(childId) || !mContext.getElement(parentId)) {
        return false;
    }

    mContext.elementAssignment().assignElementToParent(childId, parentId);
    return true;
}

bool AssignmentService::removeElementFromParent(const EntityId& childId) const {
    return mContext.elementAssignment().removeElementFromParent(childId);
}

std::vector<Element*> AssignmentService::getChildElements(const EntityId& parentId) const {
    const auto childIds = mContext.elementAssignment().getChildElements(parentId);

    std::vector<Element*> result;
    result.reserve(childIds.size());

    for (const auto& id : childIds) {
        if (auto* element = mContext.getElement(id)) {
            result.push_back(element);
        }
    }

    return result;
}

Element* AssignmentService::getParentElement(const EntityId& childId) const {
    const auto parentId = mContext.elementAssignment().getParentForElement(childId);
    if (!parentId) {
        return nullptr;
    }

    return mContext.getElement(*parentId);
}

std::vector<Element*> AssignmentService::getAllElementsRecursive(const EntityId& rootId) const {
    std::vector<Element*> result;
    
    // Add the root element itself
    if (auto* root = mContext.getElement(rootId)) {
        result.push_back(root);
        collectElementsRecursive(rootId, result);
    }

    return result;
}

void AssignmentService::collectElementsRecursive(const EntityId& elementId,
                                                  std::vector<Element*>& result) const {
    const auto childIds = mContext.elementAssignment().getChildElements(elementId);

    for (const auto& childId : childIds) {
        if (auto* child = mContext.getElement(childId)) {
            result.push_back(child);
            // Recursively collect children's children
            collectElementsRecursive(childId, result);
        }
    }
}

}  // namespace CwAPI3D::Model
