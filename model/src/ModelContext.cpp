#include "cwapi3d/model/ModelContext.h"

#include <algorithm>
#include <ranges>

namespace CwAPI3D::Model {

// ========== Entity Creation ==========

Building* ModelContext::createBuilding(std::string name) {
    auto building = std::make_unique<Building>(std::move(name));
    auto* ptr = building.get();
    mBuildings[ptr->id()] = std::move(building);
    return ptr;
}

BuildingStorey* ModelContext::createFloor(std::string name, std::optional<double> elevation) {
    auto floor = std::make_unique<BuildingStorey>(std::move(name), elevation);
    auto* ptr = floor.get();
    mFloors[ptr->id()] = std::move(floor);
    return ptr;
}

Element* ModelContext::createElement(std::string name) {
    auto element = std::make_unique<Element>(std::move(name));
    auto* ptr = element.get();
    mElements[ptr->id()] = std::move(element);
    return ptr;
}

// ========== Entity Lookup ==========

Building* ModelContext::getBuilding(const EntityId& id) const {
    const auto it = mBuildings.find(id);
    return it != mBuildings.end() ? it->second.get() : nullptr;
}

BuildingStorey* ModelContext::getFloor(const EntityId& id) const {
    const auto it = mFloors.find(id);
    return it != mFloors.end() ? it->second.get() : nullptr;
}

Element* ModelContext::getElement(const EntityId& id) const {
    const auto it = mElements.find(id);
    return it != mElements.end() ? it->second.get() : nullptr;
}

std::vector<Building*> ModelContext::getAllBuildings() const {
    std::vector<Building*> result;
    result.reserve(mBuildings.size());
    for (const auto& building : mBuildings | std::views::values) {
        result.push_back(building.get());
    }
    return result;
}

std::vector<BuildingStorey*> ModelContext::getAllFloors() const {
    std::vector<BuildingStorey*> result;
    result.reserve(mFloors.size());
    for (const auto& floor : mFloors | std::views::values) {
        result.push_back(floor.get());
    }
    return result;
}

std::vector<Element*> ModelContext::getAllElements() const {
    std::vector<Element*> result;
    result.reserve(mElements.size());
    for (const auto& element : mElements | std::views::values) {
        result.push_back(element.get());
    }
    return result;
}

// ========== Entity Deletion ==========

bool ModelContext::deleteBuilding(const EntityId& id) {
    const auto it = mBuildings.find(id);
    if (it == mBuildings.end()) {
        return false;
    }

    // Clean up relations
    mBuildingAssignment.removeBuilding(id);
    mComponentRegistry.removeAllComponents(id);

    mBuildings.erase(it);
    return true;
}

bool ModelContext::deleteFloor(const EntityId& id) {
    const auto it = mFloors.find(id);
    if (it == mFloors.end()) {
        return false;
    }

    // Clean up relations
    mFloorAssignment.removeFloor(id);
    mBuildingAssignment.removeFloorFromBuilding(id);
    mComponentRegistry.removeAllComponents(id);

    mFloors.erase(it);
    return true;
}

bool ModelContext::deleteElement(const EntityId& id) {
    const auto it = mElements.find(id);
    if (it == mElements.end()) {
        return false;
    }

    // Clean up relations and components
    mFloorAssignment.removeElementFromFloor(id);
    mElementAssignment.removeElementFromParent(id);  // Remove as child
    mElementAssignment.removeParentElement(id);      // Remove as parent (all children)
    mComponentRegistry.removeAllComponents(id);

    mElements.erase(it);
    return true;
}

// ========== Clear ==========

void ModelContext::clear() {
    mBuildings.clear();
    mFloors.clear();
    mElements.clear();
    mFloorAssignment.clear();
    mBuildingAssignment.clear();
    mElementAssignment.clear();
}

}  // namespace CwAPI3D::Model
