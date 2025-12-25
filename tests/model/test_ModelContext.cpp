#include <gtest/gtest.h>

#include "cwapi3d/model/BRepGeometry.h"
#include "cwapi3d/model/ModelContext.h"
#include "cwapi3d/model/PropertySet.h"

using namespace CwAPI3D::Model;

TEST(ModelContextTest, CreateBuilding) {
    ModelContext context;

    auto* building = context.createBuilding("Test Building");

    EXPECT_NE(building, nullptr);
    EXPECT_EQ(building->name(), "Test Building");
    EXPECT_EQ(context.buildingCount(), 1);
}

TEST(ModelContextTest, CreateFloor) {
    ModelContext context;

    auto* floor = context.createFloor("Ground Floor", 0.0);

    EXPECT_NE(floor, nullptr);
    EXPECT_EQ(floor->name(), "Ground Floor");
    EXPECT_EQ(context.floorCount(), 1);
}

TEST(ModelContextTest, CreateElement) {
    ModelContext context;

    auto* element = context.createElement("Wall");

    EXPECT_NE(element, nullptr);
    EXPECT_EQ(element->name(), "Wall");
    EXPECT_EQ(context.elementCount(), 1);
}

TEST(ModelContextTest, GetBuilding) {
    ModelContext context;

    auto* building = context.createBuilding("Building");
    auto* retrieved = context.getBuilding(building->id());

    EXPECT_EQ(building, retrieved);
}

TEST(ModelContextTest, GetNonExistentBuilding) {
    ModelContext context;
    auto fakeId = EntityId::create();

    auto* building = context.getBuilding(fakeId);
    EXPECT_EQ(building, nullptr);
}

TEST(ModelContextTest, DeleteBuilding) {
    ModelContext context;

    auto* building = context.createBuilding("Building");
    auto id = building->id();

    EXPECT_EQ(context.buildingCount(), 1);

    bool deleted = context.deleteBuilding(id);
    EXPECT_TRUE(deleted);
    EXPECT_EQ(context.buildingCount(), 0);
    EXPECT_EQ(context.getBuilding(id), nullptr);
}

TEST(ModelContextTest, DeleteNonExistentBuilding) {
    ModelContext context;
    auto fakeId = EntityId::create();

    bool deleted = context.deleteBuilding(fakeId);
    EXPECT_FALSE(deleted);
}

TEST(ModelContextTest, DeleteFloorCleansUpRelations) {
    ModelContext context;

    auto* floor = context.createFloor("Floor");
    auto* element = context.createElement("Element");

    context.floorAssignment().assignElementToFloor(element->id(), floor->id());

    EXPECT_TRUE(context.floorAssignment().getFloorForElement(element->id()).has_value());

    context.deleteFloor(floor->id());

    EXPECT_FALSE(context.floorAssignment().getFloorForElement(element->id()).has_value());
}

TEST(ModelContextTest, DeleteElementCleansUpComponents) {
    ModelContext context;

    auto* element = context.createElement("Element");

    context.componentRegistry().addComponent(element->id(), std::make_unique<BRepGeometry>());

    EXPECT_TRUE(context.componentRegistry().hasComponent<BRepGeometry>(element->id()));

    context.deleteElement(element->id());

    EXPECT_FALSE(context.componentRegistry().hasComponent<BRepGeometry>(element->id()));
}

TEST(ModelContextTest, GetAllBuildings) {
    ModelContext context;

    context.createBuilding("Building1");
    context.createBuilding("Building2");
    context.createBuilding("Building3");

    auto buildings = context.getAllBuildings();
    EXPECT_EQ(buildings.size(), 3);
}

TEST(ModelContextTest, GetAllFloors) {
    ModelContext context;

    context.createFloor("Floor1");
    context.createFloor("Floor2");

    auto floors = context.getAllFloors();
    EXPECT_EQ(floors.size(), 2);
}

TEST(ModelContextTest, GetAllElements) {
    ModelContext context;

    context.createElement("Element1");
    context.createElement("Element2");
    context.createElement("Element3");
    context.createElement("Element4");

    auto elements = context.getAllElements();
    EXPECT_EQ(elements.size(), 4);
}

TEST(ModelContextTest, Clear) {
    ModelContext context;

    context.createBuilding("Building");
    context.createFloor("Floor");
    context.createElement("Element");

    EXPECT_EQ(context.buildingCount(), 1);
    EXPECT_EQ(context.floorCount(), 1);
    EXPECT_EQ(context.elementCount(), 1);

    context.clear();

    EXPECT_EQ(context.buildingCount(), 0);
    EXPECT_EQ(context.floorCount(), 0);
    EXPECT_EQ(context.elementCount(), 0);
}

TEST(ModelContextTest, ComponentRegistryAccess) {
    ModelContext context;

    auto& registry = context.componentRegistry();
    auto entityId = EntityId::create();

    registry.addComponent(entityId, std::make_unique<BRepGeometry>());

    EXPECT_TRUE(registry.hasComponent<BRepGeometry>(entityId));
}

TEST(ModelContextTest, FloorAssignmentAccess) {
    ModelContext context;

    auto& assignments = context.floorAssignment();
    auto elementId = EntityId::create();
    auto floorId = EntityId::create();

    assignments.assignElementToFloor(elementId, floorId);

    EXPECT_TRUE(assignments.getFloorForElement(elementId).has_value());
}
