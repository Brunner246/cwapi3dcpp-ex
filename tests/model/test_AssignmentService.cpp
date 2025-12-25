#include <gtest/gtest.h>

#include "cwapi3d/model/AssignmentService.h"
#include "cwapi3d/model/BRepGeometry.h"
#include "cwapi3d/model/ModelContext.h"
#include "cwapi3d/model/PropertySet.h"

using namespace CwAPI3D::Model;

class AssignmentServiceTest : public ::testing::Test {
   protected:
    ModelContext context;
    AssignmentService service{context};
};

TEST_F(AssignmentServiceTest, AssignElementToFloor) {
    auto* element = context.createElement("Element");
    auto* floor = context.createFloor("Floor");

    bool success = service.assignElementToFloor(element->id(), floor->id());
    EXPECT_TRUE(success);

    auto assignedFloor = context.floorAssignment().getFloorForElement(element->id());
    EXPECT_TRUE(assignedFloor.has_value());
    EXPECT_EQ(assignedFloor.value(), floor->id());
}

TEST_F(AssignmentServiceTest, AssignWithInvalidElement) {
    auto* floor = context.createFloor("Floor");
    auto fakeElementId = EntityId::create();

    bool success = service.assignElementToFloor(fakeElementId, floor->id());
    EXPECT_FALSE(success);
}

TEST_F(AssignmentServiceTest, AssignWithInvalidFloor) {
    auto* element = context.createElement("Element");
    auto fakeFloorId = EntityId::create();

    bool success = service.assignElementToFloor(element->id(), fakeFloorId);
    EXPECT_FALSE(success);
}

TEST_F(AssignmentServiceTest, GetElementsOnFloor) {
    auto* floor = context.createFloor("Floor");
    auto* elem1 = context.createElement("Element1");
    auto* elem2 = context.createElement("Element2");

    std::ignore = service.assignElementToFloor(elem1->id(), floor->id());
    std::ignore = service.assignElementToFloor(elem2->id(), floor->id());

    auto elements = service.getElementsOnFloor(floor->id());
    EXPECT_EQ(elements.size(), 2);
}

TEST_F(AssignmentServiceTest, AssignFloorToBuilding) {
    auto* building = context.createBuilding("Building");
    auto* floor = context.createFloor("Floor");

    bool success = service.assignFloorToBuilding(floor->id(), building->id());
    EXPECT_TRUE(success);

    auto assignedBuilding = context.buildingAssignment().getBuildingForFloor(floor->id());
    EXPECT_TRUE(assignedBuilding.has_value());
    EXPECT_EQ(assignedBuilding.value(), building->id());
}

TEST_F(AssignmentServiceTest, GetFloorsInBuilding) {
    auto* building = context.createBuilding("Building");
    auto* floor1 = context.createFloor("Floor1");
    auto* floor2 = context.createFloor("Floor2");

    std::ignore = service.assignFloorToBuilding(floor1->id(), building->id());
    std::ignore = service.assignFloorToBuilding(floor2->id(), building->id());

    auto floors = service.getFloorsInBuilding(building->id());
    EXPECT_EQ(floors.size(), 2);
}


TEST_F(AssignmentServiceTest, GetElementsInBuilding) {
    auto* building = context.createBuilding("Building");
    auto* floor1 = context.createFloor("Floor1");
    auto* floor2 = context.createFloor("Floor2");

    auto* elem1 = context.createElement("Element1");
    auto* elem2 = context.createElement("Element2");
    auto* elem3 = context.createElement("Element3");

    std::ignore = service.assignFloorToBuilding(floor1->id(), building->id());
    std::ignore = service.assignFloorToBuilding(floor2->id(), building->id());

    std::ignore = service.assignElementToFloor(elem1->id(), floor1->id());
    std::ignore = service.assignElementToFloor(elem2->id(), floor1->id());
    std::ignore = service.assignElementToFloor(elem3->id(), floor2->id());

    auto elements = service.getElementsInBuilding(building->id());
    EXPECT_EQ(elements.size(), 3);
}

TEST_F(AssignmentServiceTest, GetBuildingForElement) {
    auto* building = context.createBuilding("Building");
    auto* floor = context.createFloor("Floor");
    auto* element = context.createElement("Element");

    std::ignore = service.assignFloorToBuilding(floor->id(), building->id());
    std::ignore = service.assignElementToFloor(element->id(), floor->id());

    auto* retrievedBuilding = service.getBuildingForElement(element->id());
    EXPECT_NE(retrievedBuilding, nullptr);
    EXPECT_EQ(retrievedBuilding->id(), building->id());
}

TEST_F(AssignmentServiceTest, GetBuildingForUnassignedElement) {
    auto* element = context.createElement("Element");

    auto* building = service.getBuildingForElement(element->id());
    EXPECT_EQ(building, nullptr);
}
