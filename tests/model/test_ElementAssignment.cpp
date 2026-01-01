#include <gtest/gtest.h>

#include "cwapi3d/model/ElementAssignment.h"
#include "cwapi3d/model/EntityId.h"

using namespace CwAPI3D::Model;

TEST(ElementAssignmentTest, AssignElementToParent) {
    ElementAssignment assignments;

    const auto childId = EntityId::create();
    const auto parentId = EntityId::create();

    assignments.assignElementToParent(childId, parentId);

    const auto assignedParent = assignments.getParentForElement(childId);
    EXPECT_TRUE(assignedParent.has_value());
    EXPECT_EQ(assignedParent.value(), parentId);
}

TEST(ElementAssignmentTest, GetUnassignedElement) {
    const ElementAssignment assignments;
    const auto elementId = EntityId::create();

    const auto parent = assignments.getParentForElement(elementId);
    EXPECT_FALSE(parent.has_value());
}

TEST(ElementAssignmentTest, ReassignElement) {
    ElementAssignment assignments;

    const auto childId = EntityId::create();
    const auto parent1Id = EntityId::create();
    const auto parent2Id = EntityId::create();

    // Assign to first parent
    assignments.assignElementToParent(childId, parent1Id);
    EXPECT_EQ(assignments.getChildElementCount(parent1Id), 1);

    // Reassign to second parent
    assignments.assignElementToParent(childId, parent2Id);

    // Should be removed from first parent
    EXPECT_EQ(assignments.getChildElementCount(parent1Id), 0);

    // Should be assigned to second parent
    const auto assignedParent = assignments.getParentForElement(childId);
    EXPECT_TRUE(assignedParent.has_value());
    EXPECT_EQ(assignedParent.value(), parent2Id);
    EXPECT_EQ(assignments.getChildElementCount(parent2Id), 1);
}

TEST(ElementAssignmentTest, RemoveElement) {
    ElementAssignment assignments;

    const auto childId = EntityId::create();
    const auto parentId = EntityId::create();

    assignments.assignElementToParent(childId, parentId);
    EXPECT_EQ(assignments.getChildElementCount(parentId), 1);

    const bool removed = assignments.removeElementFromParent(childId);
    EXPECT_TRUE(removed);

    const auto parent = assignments.getParentForElement(childId);
    EXPECT_FALSE(parent.has_value());
    EXPECT_EQ(assignments.getChildElementCount(parentId), 0);
}

TEST(ElementAssignmentTest, RemoveUnassignedElement) {
    ElementAssignment assignments;
    const auto elementId = EntityId::create();

    const bool removed = assignments.removeElementFromParent(elementId);
    EXPECT_FALSE(removed);
}

TEST(ElementAssignmentTest, GetChildElements) {
    ElementAssignment assignments;

    const auto parentId = EntityId::create();
    const auto child1Id = EntityId::create();
    const auto child2Id = EntityId::create();
    const auto child3Id = EntityId::create();

    assignments.assignElementToParent(child1Id, parentId);
    assignments.assignElementToParent(child2Id, parentId);
    assignments.assignElementToParent(child3Id, parentId);

    const auto children = assignments.getChildElements(parentId);
    EXPECT_EQ(children.size(), 3);
    EXPECT_TRUE(std::find(children.begin(), children.end(), child1Id) != children.end());
    EXPECT_TRUE(std::find(children.begin(), children.end(), child2Id) != children.end());
    EXPECT_TRUE(std::find(children.begin(), children.end(), child3Id) != children.end());
}

TEST(ElementAssignmentTest, GetChildElementCount) {
    ElementAssignment assignments;

    const auto parentId = EntityId::create();
    const auto child1Id = EntityId::create();
    const auto child2Id = EntityId::create();

    EXPECT_EQ(assignments.getChildElementCount(parentId), 0);

    assignments.assignElementToParent(child1Id, parentId);
    EXPECT_EQ(assignments.getChildElementCount(parentId), 1);

    assignments.assignElementToParent(child2Id, parentId);
    EXPECT_EQ(assignments.getChildElementCount(parentId), 2);

    assignments.removeElementFromParent(child1Id);
    EXPECT_EQ(assignments.getChildElementCount(parentId), 1);
}

TEST(ElementAssignmentTest, IsElementChildOf) {
    ElementAssignment assignments;

    const auto childId = EntityId::create();
    const auto parentId = EntityId::create();
    const auto otherParentId = EntityId::create();

    assignments.assignElementToParent(childId, parentId);

    EXPECT_TRUE(assignments.isElementChildOf(childId, parentId));
    EXPECT_FALSE(assignments.isElementChildOf(childId, otherParentId));
}

TEST(ElementAssignmentTest, RemoveParentElement) {
    ElementAssignment assignments;

    const auto parentId = EntityId::create();
    const auto child1Id = EntityId::create();
    const auto child2Id = EntityId::create();
    const auto child3Id = EntityId::create();

    assignments.assignElementToParent(child1Id, parentId);
    assignments.assignElementToParent(child2Id, parentId);
    assignments.assignElementToParent(child3Id, parentId);

    EXPECT_EQ(assignments.getChildElementCount(parentId), 3);

    assignments.removeParentElement(parentId);

    EXPECT_EQ(assignments.getChildElementCount(parentId), 0);
    EXPECT_FALSE(assignments.getParentForElement(child1Id).has_value());
    EXPECT_FALSE(assignments.getParentForElement(child2Id).has_value());
    EXPECT_FALSE(assignments.getParentForElement(child3Id).has_value());
}

TEST(ElementAssignmentTest, GetParentsWithChildren) {
    ElementAssignment assignments;

    const auto parent1Id = EntityId::create();
    const auto parent2Id = EntityId::create();
    const auto child1Id = EntityId::create();
    const auto child2Id = EntityId::create();

    assignments.assignElementToParent(child1Id, parent1Id);
    assignments.assignElementToParent(child2Id, parent2Id);

    const auto parents = assignments.getParentsWithChildren();
    EXPECT_EQ(parents.size(), 2);
    EXPECT_TRUE(std::find(parents.begin(), parents.end(), parent1Id) != parents.end());
    EXPECT_TRUE(std::find(parents.begin(), parents.end(), parent2Id) != parents.end());
}

TEST(ElementAssignmentTest, PreventSelfReference) {
    ElementAssignment assignments;

    const auto elementId = EntityId::create();

    // Try to assign element as its own parent
    assignments.assignElementToParent(elementId, elementId);

    // Should not be assigned
    const auto parent = assignments.getParentForElement(elementId);
    EXPECT_FALSE(parent.has_value());
    EXPECT_EQ(assignments.getChildElementCount(elementId), 0);
}

TEST(ElementAssignmentTest, Clear) {
    ElementAssignment assignments;

    const auto parent1Id = EntityId::create();
    const auto parent2Id = EntityId::create();
    const auto child1Id = EntityId::create();
    const auto child2Id = EntityId::create();

    assignments.assignElementToParent(child1Id, parent1Id);
    assignments.assignElementToParent(child2Id, parent2Id);

    assignments.clear();

    EXPECT_FALSE(assignments.getParentForElement(child1Id).has_value());
    EXPECT_FALSE(assignments.getParentForElement(child2Id).has_value());
    EXPECT_EQ(assignments.getChildElementCount(parent1Id), 0);
    EXPECT_EQ(assignments.getChildElementCount(parent2Id), 0);
    EXPECT_TRUE(assignments.getParentsWithChildren().empty());
}
