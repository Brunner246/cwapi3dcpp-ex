#include <gtest/gtest.h>

#include "cwapi3d/model/BuildingStoreyAssignment.h"
#include "cwapi3d/model/EntityId.h"

using namespace CwAPI3D::Model;

TEST(FloorAssignmentTest, AssignElement) {
    BuildingStoreyAssignment assignments;

    const auto elementId = EntityId::create();
    const auto floorId = EntityId::create();

    assignments.assignElementToFloor(elementId, floorId);

    const auto assignedFloor = assignments.getFloorForElement(elementId);
    EXPECT_TRUE(assignedFloor.has_value());
    EXPECT_EQ(assignedFloor.value(), floorId);
}

TEST(FloorAssignmentTest, GetUnassignedElement) {
    const BuildingStoreyAssignment assignments;
    const auto elementId = EntityId::create();

    const auto floor = assignments.getFloorForElement(elementId);
    EXPECT_FALSE(floor.has_value());
}

TEST(FloorAssignmentTest, ReassignElement) {
    BuildingStoreyAssignment assignments;

    const auto elementId = EntityId::create();
    const auto floor1 = EntityId::create();
    const auto floor2 = EntityId::create();

    assignments.assignElementToFloor(elementId, floor1);
    EXPECT_EQ(assignments.getFloorForElement(elementId).value(), floor1);

    assignments.assignElementToFloor(elementId, floor2);
    EXPECT_EQ(assignments.getFloorForElement(elementId).value(), floor2);

    // Old floor should no longer have the element
    const auto elements = assignments.getElementsOnFloor(floor1);
    EXPECT_EQ(elements.size(), 0);
}

TEST(FloorAssignmentTest, RemoveElement) {
    BuildingStoreyAssignment assignments;

    const auto elementId = EntityId::create();
    const auto floorId = EntityId::create();

    assignments.assignElementToFloor(elementId, floorId);
    EXPECT_TRUE(assignments.getFloorForElement(elementId).has_value());

    const bool removed = assignments.removeElementFromFloor(elementId);
    EXPECT_TRUE(removed);
    EXPECT_FALSE(assignments.getFloorForElement(elementId).has_value());
}

TEST(FloorAssignmentTest, RemoveUnassignedElement) {
    BuildingStoreyAssignment assignments;
    const auto elementId = EntityId::create();

    const bool removed = assignments.removeElementFromFloor(elementId);
    EXPECT_FALSE(removed);
}

TEST(FloorAssignmentTest, GetElementsOnFloor) {
    BuildingStoreyAssignment assignments;

    const auto floorId = EntityId::create();
    const auto elem1 = EntityId::create();
    const auto elem2 = EntityId::create();
    const auto elem3 = EntityId::create();

    assignments.assignElementToFloor(elem1, floorId);
    assignments.assignElementToFloor(elem2, floorId);
    assignments.assignElementToFloor(elem3, floorId);

    const auto elements = assignments.getElementsOnFloor(floorId);
    EXPECT_EQ(elements.size(), 3);
}

TEST(FloorAssignmentTest, GetElementCountOnFloor) {
    BuildingStoreyAssignment assignments;

    const auto floorId = EntityId::create();
    const auto elem1 = EntityId::create();
    const auto elem2 = EntityId::create();

    EXPECT_EQ(assignments.getElementCountOnFloor(floorId), 0);

    assignments.assignElementToFloor(elem1, floorId);
    EXPECT_EQ(assignments.getElementCountOnFloor(floorId), 1);

    assignments.assignElementToFloor(elem2, floorId);
    EXPECT_EQ(assignments.getElementCountOnFloor(floorId), 2);
}

TEST(FloorAssignmentTest, IsElementOnFloor) {
    BuildingStoreyAssignment assignments;

    const auto elementId = EntityId::create();
    const auto floor1 = EntityId::create();
    const auto floor2 = EntityId::create();

    assignments.assignElementToFloor(elementId, floor1);

    EXPECT_TRUE(assignments.isElementOnFloor(elementId, floor1));
    EXPECT_FALSE(assignments.isElementOnFloor(elementId, floor2));
}

TEST(FloorAssignmentTest, RemoveFloor) {
    BuildingStoreyAssignment assignments;

    const auto floorId = EntityId::create();
    const auto elem1 = EntityId::create();
    const auto elem2 = EntityId::create();

    assignments.assignElementToFloor(elem1, floorId);
    assignments.assignElementToFloor(elem2, floorId);

    assignments.removeFloor(floorId);

    EXPECT_FALSE(assignments.getFloorForElement(elem1).has_value());
    EXPECT_FALSE(assignments.getFloorForElement(elem2).has_value());
    EXPECT_EQ(assignments.getElementsOnFloor(floorId).size(), 0);
}

TEST(FloorAssignmentTest, GetFloorsWithElements) {
    BuildingStoreyAssignment assignments;

    const auto floor1 = EntityId::create();
    const auto floor2 = EntityId::create();
    const auto elem1 = EntityId::create();
    const auto elem2 = EntityId::create();

    assignments.assignElementToFloor(elem1, floor1);
    assignments.assignElementToFloor(elem2, floor2);

    const auto floors = assignments.getFloorsWithElements();
    EXPECT_EQ(floors.size(), 2);
}

TEST(FloorAssignmentTest, Clear) {
    BuildingStoreyAssignment assignments;

    const auto floorId = EntityId::create();
    const auto elem1 = EntityId::create();
    const auto elem2 = EntityId::create();

    assignments.assignElementToFloor(elem1, floorId);
    assignments.assignElementToFloor(elem2, floorId);

    assignments.clear();

    EXPECT_FALSE(assignments.getFloorForElement(elem1).has_value());
    EXPECT_EQ(assignments.getElementsOnFloor(floorId).size(), 0);
}
