#include <gtest/gtest.h>

#include "cwapi3d/model/AssignmentService.h"
#include "cwapi3d/model/ModelContext.h"

using namespace CwAPI3D::Model;

class ElementHierarchyTest : public ::testing::Test {
   protected:
    ModelContext context;
    AssignmentService service{context};
};

TEST_F(ElementHierarchyTest, AssignElementToParent) {
    const auto* wall = context.createElement("Wall");
    const auto* post = context.createElement("Post");

    const bool success = service.assignElementToParent(post->id(), wall->id());
    EXPECT_TRUE(success);

    auto* assignedParent = service.getParentElement(post->id());
    EXPECT_NE(assignedParent, nullptr);
    EXPECT_EQ(assignedParent->id(), wall->id());
}

TEST_F(ElementHierarchyTest, AssignWithInvalidChild) {
    const auto* wall = context.createElement("Wall");
    const EntityId invalidId = EntityId::create();

    const bool success = service.assignElementToParent(invalidId, wall->id());
    EXPECT_FALSE(success);
}

TEST_F(ElementHierarchyTest, AssignWithInvalidParent) {
    const auto* post = context.createElement("Post");
    const EntityId invalidId = EntityId::create();

    const bool success = service.assignElementToParent(post->id(), invalidId);
    EXPECT_FALSE(success);
}

TEST_F(ElementHierarchyTest, GetChildElements) {
    const auto* wall = context.createElement("Wall");
    const auto* post1 = context.createElement("Post1");
    const auto* post2 = context.createElement("Post2");
    const auto* plate = context.createElement("Plate");

    std::ignore = service.assignElementToParent(post1->id(), wall->id());
    std::ignore = service.assignElementToParent(post2->id(), wall->id());
    std::ignore = service.assignElementToParent(plate->id(), wall->id());

    const auto children = service.getChildElements(wall->id());
    EXPECT_EQ(children.size(), 3);

    // Verify all children are present
    std::vector<EntityId> childIds;
    for (const auto* child : children) {
        childIds.push_back(child->id());
    }

    EXPECT_TRUE(std::find(childIds.begin(), childIds.end(), post1->id()) != childIds.end());
    EXPECT_TRUE(std::find(childIds.begin(), childIds.end(), post2->id()) != childIds.end());
    EXPECT_TRUE(std::find(childIds.begin(), childIds.end(), plate->id()) != childIds.end());
}

TEST_F(ElementHierarchyTest, WallWithPostsAndPlatesScenario) {
    // Create building structure
    const auto* building = context.createBuilding("Office Building");
    const auto* floor = context.createFloor("Ground Floor", 0.0);

    // Create wall with sub-elements
    const auto* wall = context.createElement("Wall");
    const auto* topPlate = context.createElement("Top Plate");
    const auto* bottomPlate = context.createElement("Bottom Plate");
    const auto* post1 = context.createElement("Post 1");
    const auto* post2 = context.createElement("Post 2");
    const auto* post3 = context.createElement("Post 3");

    // Establish hierarchy: Building -> Floor -> Wall -> {Posts, Plates}
    std::ignore = service.assignFloorToBuilding(floor->id(), building->id());
    std::ignore = service.assignElementToFloor(wall->id(), floor->id());

    // Assign sub-elements to wall
    std::ignore = service.assignElementToParent(topPlate->id(), wall->id());
    std::ignore = service.assignElementToParent(bottomPlate->id(), wall->id());
    std::ignore = service.assignElementToParent(post1->id(), wall->id());
    std::ignore = service.assignElementToParent(post2->id(), wall->id());
    std::ignore = service.assignElementToParent(post3->id(), wall->id());

    // Verify the hierarchy
    const auto children = service.getChildElements(wall->id());
    EXPECT_EQ(children.size(), 5);

    // Verify wall is on the correct floor
    const auto floorsInBuilding = service.getFloorsInBuilding(building->id());
    EXPECT_EQ(floorsInBuilding.size(), 1);
    EXPECT_EQ(floorsInBuilding[0]->id(), floor->id());

    // Verify wall is an element on the floor
    const auto elementsOnFloor = service.getElementsOnFloor(floor->id());
    EXPECT_EQ(elementsOnFloor.size(), 1);
    EXPECT_EQ(elementsOnFloor[0]->id(), wall->id());

    // Verify each sub-element has wall as parent
    EXPECT_EQ(service.getParentElement(topPlate->id())->id(), wall->id());
    EXPECT_EQ(service.getParentElement(bottomPlate->id())->id(), wall->id());
    EXPECT_EQ(service.getParentElement(post1->id())->id(), wall->id());
    EXPECT_EQ(service.getParentElement(post2->id())->id(), wall->id());
    EXPECT_EQ(service.getParentElement(post3->id())->id(), wall->id());
}

TEST_F(ElementHierarchyTest, RecursiveElementHierarchy) {
    // Create a nested hierarchy: Wall -> Frame -> Studs
    const auto* wall = context.createElement("Wall");
    const auto* frame1 = context.createElement("Frame1");
    const auto* frame2 = context.createElement("Frame2");
    const auto* stud1 = context.createElement("Stud1");
    const auto* stud2 = context.createElement("Stud2");
    const auto* stud3 = context.createElement("Stud3");

    // Level 1: Frames in Wall
    std::ignore = service.assignElementToParent(frame1->id(), wall->id());
    std::ignore = service.assignElementToParent(frame2->id(), wall->id());

    // Level 2: Studs in Frames
    std::ignore = service.assignElementToParent(stud1->id(), frame1->id());
    std::ignore = service.assignElementToParent(stud2->id(), frame1->id());
    std::ignore = service.assignElementToParent(stud3->id(), frame2->id());

    // Verify immediate children
    const auto wallChildren = service.getChildElements(wall->id());
    EXPECT_EQ(wallChildren.size(), 2);

    const auto frame1Children = service.getChildElements(frame1->id());
    EXPECT_EQ(frame1Children.size(), 2);

    const auto frame2Children = service.getChildElements(frame2->id());
    EXPECT_EQ(frame2Children.size(), 1);

    // Verify recursive retrieval
    const auto allElements = service.getAllElementsRecursive(wall->id());
    EXPECT_EQ(allElements.size(), 6);  // Wall + 2 Frames + 3 Studs
}

TEST_F(ElementHierarchyTest, RemoveElementFromParent) {
    const auto* wall = context.createElement("Wall");
    const auto* post = context.createElement("Post");

    std::ignore = service.assignElementToParent(post->id(), wall->id());
    EXPECT_EQ(service.getChildElements(wall->id()).size(), 1);

    const bool removed = service.removeElementFromParent(post->id());
    EXPECT_TRUE(removed);
    EXPECT_EQ(service.getChildElements(wall->id()).size(), 0);
    EXPECT_EQ(service.getParentElement(post->id()), nullptr);
}

TEST_F(ElementHierarchyTest, DeleteParentElementCleansUpRelations) {
    const auto* wall = context.createElement("Wall");
    const auto* post1 = context.createElement("Post1");
    const auto* post2 = context.createElement("Post2");

    std::ignore = service.assignElementToParent(post1->id(), wall->id());
    std::ignore = service.assignElementToParent(post2->id(), wall->id());

    EXPECT_EQ(service.getChildElements(wall->id()).size(), 2);

    // Delete the parent wall
    const EntityId wallId = wall->id();
    const bool deleted = context.deleteElement(wallId);
    EXPECT_TRUE(deleted);

    // Children should no longer have a parent
    EXPECT_EQ(service.getParentElement(post1->id()), nullptr);
    EXPECT_EQ(service.getParentElement(post2->id()), nullptr);
}

TEST_F(ElementHierarchyTest, DeleteChildElement) {
    const auto* wall = context.createElement("Wall");
    const auto* post1 = context.createElement("Post1");
    const auto* post2 = context.createElement("Post2");

    std::ignore = service.assignElementToParent(post1->id(), wall->id());
    std::ignore = service.assignElementToParent(post2->id(), wall->id());

    // Delete one child
    const EntityId post1Id = post1->id();
    const bool deleted = context.deleteElement(post1Id);
    EXPECT_TRUE(deleted);

    // Wall should have only one child left
    const auto children = service.getChildElements(wall->id());
    EXPECT_EQ(children.size(), 1);
    EXPECT_EQ(children[0]->id(), post2->id());
}

TEST_F(ElementHierarchyTest, FullHierarchyWithMultipleLevels) {
    // Create a complete hierarchy: Building -> Floor -> Walls -> Components
    const auto* building = context.createBuilding("My Building");
    const auto* floor1 = context.createFloor("Floor 1", 0.0);
    const auto* floor2 = context.createFloor("Floor 2", 3.5);

    // Floor 1 walls
    const auto* wall1 = context.createElement("Wall 1");
    const auto* wall2 = context.createElement("Wall 2");

    // Floor 2 walls
    const auto* wall3 = context.createElement("Wall 3");

    // Wall 1 components
    const auto* wall1Post1 = context.createElement("Wall1-Post1");
    const auto* wall1Post2 = context.createElement("Wall1-Post2");

    // Wall 2 components
    const auto* wall2Plate = context.createElement("Wall2-Plate");

    // Establish building hierarchy
    std::ignore = service.assignFloorToBuilding(floor1->id(), building->id());
    std::ignore = service.assignFloorToBuilding(floor2->id(), building->id());

    std::ignore = service.assignElementToFloor(wall1->id(), floor1->id());
    std::ignore = service.assignElementToFloor(wall2->id(), floor1->id());
    std::ignore = service.assignElementToFloor(wall3->id(), floor2->id());

    std::ignore = service.assignElementToParent(wall1Post1->id(), wall1->id());
    std::ignore = service.assignElementToParent(wall1Post2->id(), wall1->id());
    std::ignore = service.assignElementToParent(wall2Plate->id(), wall2->id());

    // Verify structure
    EXPECT_EQ(service.getFloorsInBuilding(building->id()).size(), 2);
    EXPECT_EQ(service.getElementsOnFloor(floor1->id()).size(), 2);
    EXPECT_EQ(service.getElementsOnFloor(floor2->id()).size(), 1);
    EXPECT_EQ(service.getChildElements(wall1->id()).size(), 2);
    EXPECT_EQ(service.getChildElements(wall2->id()).size(), 1);
    EXPECT_EQ(service.getChildElements(wall3->id()).size(), 0);

    // Verify recursive retrieval from wall1
    const auto wall1AllElements = service.getAllElementsRecursive(wall1->id());
    EXPECT_EQ(wall1AllElements.size(), 3);  // Wall1 + 2 posts
}

TEST_F(ElementHierarchyTest, ReassignElementToNewParent) {
    const auto* wall1 = context.createElement("Wall1");
    const auto* wall2 = context.createElement("Wall2");
    const auto* post = context.createElement("Post");

    // Assign to wall1
    std::ignore = service.assignElementToParent(post->id(), wall1->id());
    EXPECT_EQ(service.getParentElement(post->id())->id(), wall1->id());
    EXPECT_EQ(service.getChildElements(wall1->id()).size(), 1);

    // Reassign to wall2
    std::ignore = service.assignElementToParent(post->id(), wall2->id());
    EXPECT_EQ(service.getParentElement(post->id())->id(), wall2->id());
    EXPECT_EQ(service.getChildElements(wall1->id()).size(), 0);
    EXPECT_EQ(service.getChildElements(wall2->id()).size(), 1);
}

TEST_F(ElementHierarchyTest, ElementCanBeInFloorAndHaveChildren) {
    // An element can be both assigned to a floor AND have child elements
    const auto* floor = context.createFloor("Floor", 0.0);
    const auto* wall = context.createElement("Wall");
    const auto* post = context.createElement("Post");

    // Wall is on floor
    std::ignore = service.assignElementToFloor(wall->id(), floor->id());

    // Wall also contains post
    std::ignore = service.assignElementToParent(post->id(), wall->id());

    // Verify both relationships
    const auto elementsOnFloor = service.getElementsOnFloor(floor->id());
    EXPECT_EQ(elementsOnFloor.size(), 1);
    EXPECT_EQ(elementsOnFloor[0]->id(), wall->id());

    const auto wallChildren = service.getChildElements(wall->id());
    EXPECT_EQ(wallChildren.size(), 1);
    EXPECT_EQ(wallChildren[0]->id(), post->id());
}

TEST_F(ElementHierarchyTest, BuildingWithStoreyContainingWallWithBeamsAndPanels) {
    // Create the building
    auto* building = context.createBuilding("Commercial Building");
    ASSERT_NE(building, nullptr);
    EXPECT_EQ(building->name(), "Commercial Building");

    // Create the storey
    auto* groundStorey = context.createFloor("Ground Storey", 0.0);
    ASSERT_NE(groundStorey, nullptr);
    EXPECT_EQ(groundStorey->name(), "Ground Storey");
    EXPECT_TRUE(groundStorey->elevation().has_value());
    EXPECT_DOUBLE_EQ(groundStorey->elevation().value(), 0.0);

    // Assign storey to building
    bool assignedToBuilding = service.assignFloorToBuilding(groundStorey->id(), building->id());
    EXPECT_TRUE(assignedToBuilding);

    // Create the wall
    auto* wall = context.createElement("Exterior Wall");
    ASSERT_NE(wall, nullptr);
    EXPECT_EQ(wall->name(), "Exterior Wall");

    // Assign wall to storey
    bool assignedToStorey = service.assignElementToFloor(wall->id(), groundStorey->id());
    EXPECT_TRUE(assignedToStorey);

    // Create beams and panels as sub-elements of the wall
    auto* beam1 = context.createElement("Horizontal Beam 1");
    auto* beam2 = context.createElement("Horizontal Beam 2");
    auto* beam3 = context.createElement("Vertical Beam");
    auto* panel1 = context.createElement("Insulation Panel 1");
    auto* panel2 = context.createElement("Insulation Panel 2");
    auto* panel3 = context.createElement("Cladding Panel");

    ASSERT_NE(beam1, nullptr);
    ASSERT_NE(beam2, nullptr);
    ASSERT_NE(beam3, nullptr);
    ASSERT_NE(panel1, nullptr);
    ASSERT_NE(panel2, nullptr);
    ASSERT_NE(panel3, nullptr);

    // Assign beams to wall
    EXPECT_TRUE(service.assignElementToParent(beam1->id(), wall->id()));
    EXPECT_TRUE(service.assignElementToParent(beam2->id(), wall->id()));
    EXPECT_TRUE(service.assignElementToParent(beam3->id(), wall->id()));

    // Assign panels to wall
    EXPECT_TRUE(service.assignElementToParent(panel1->id(), wall->id()));
    EXPECT_TRUE(service.assignElementToParent(panel2->id(), wall->id()));
    EXPECT_TRUE(service.assignElementToParent(panel3->id(), wall->id()));

    // Verify the complete hierarchy
    // 1. Building contains the storey
    auto storeysInBuilding = service.getFloorsInBuilding(building->id());
    EXPECT_EQ(storeysInBuilding.size(), 1);
    EXPECT_EQ(storeysInBuilding[0]->id(), groundStorey->id());
    EXPECT_EQ(storeysInBuilding[0]->name(), "Ground Storey");

    // 2. Storey contains the wall
    auto elementsInStorey = service.getElementsOnFloor(groundStorey->id());
    EXPECT_EQ(elementsInStorey.size(), 1);
    EXPECT_EQ(elementsInStorey[0]->id(), wall->id());
    EXPECT_EQ(elementsInStorey[0]->name(), "Exterior Wall");

    // 3. Wall contains beams and panels
    auto wallComponents = service.getChildElements(wall->id());
    EXPECT_EQ(wallComponents.size(), 6);  // 3 beams + 3 panels

    // Verify each beam is a child of the wall
    EXPECT_EQ(service.getParentElement(beam1->id())->id(), wall->id());
    EXPECT_EQ(service.getParentElement(beam2->id())->id(), wall->id());
    EXPECT_EQ(service.getParentElement(beam3->id())->id(), wall->id());

    // Verify each panel is a child of the wall
    EXPECT_EQ(service.getParentElement(panel1->id())->id(), wall->id());
    EXPECT_EQ(service.getParentElement(panel2->id())->id(), wall->id());
    EXPECT_EQ(service.getParentElement(panel3->id())->id(), wall->id());

    // Verify names are preserved
    EXPECT_EQ(beam1->name(), "Horizontal Beam 1");
    EXPECT_EQ(panel3->name(), "Cladding Panel");
}

TEST_F(ElementHierarchyTest, MultipleWallsWithDifferentComponents) {
    // Create building structure
    const auto* building = context.createBuilding("Residential Building");
    const auto* firstStorey = context.createFloor("First Storey", 3.0);

    std::ignore = service.assignFloorToBuilding(firstStorey->id(), building->id());

    // Create multiple walls
    const auto* wallNorth = context.createElement("North Wall");
    const auto* wallSouth = context.createElement("South Wall");
    const auto* wallEast = context.createElement("East Wall");

    // Assign all walls to the same storey
    std::ignore = service.assignElementToFloor(wallNorth->id(), firstStorey->id());
    std::ignore = service.assignElementToFloor(wallSouth->id(), firstStorey->id());
    std::ignore = service.assignElementToFloor(wallEast->id(), firstStorey->id());

    // North Wall components
    const auto* northBeam1 = context.createElement("North-Beam-1");
    const auto* northBeam2 = context.createElement("North-Beam-2");
    const auto* northPanel = context.createElement("North-Panel");
    std::ignore = service.assignElementToParent(northBeam1->id(), wallNorth->id());
    std::ignore = service.assignElementToParent(northBeam2->id(), wallNorth->id());
    std::ignore = service.assignElementToParent(northPanel->id(), wallNorth->id());

    // South Wall components
    const auto* southBeam = context.createElement("South-Beam");
    const auto* southPanel1 = context.createElement("South-Panel-1");
    const auto* southPanel2 = context.createElement("South-Panel-2");
    std::ignore = service.assignElementToParent(southBeam->id(), wallSouth->id());
    std::ignore = service.assignElementToParent(southPanel1->id(), wallSouth->id());
    std::ignore = service.assignElementToParent(southPanel2->id(), wallSouth->id());

    // East Wall components
    const auto* eastBeam = context.createElement("East-Beam");
    std::ignore = service.assignElementToParent(eastBeam->id(), wallEast->id());

    // Verify storey contains all walls
    const auto wallsInStorey = service.getElementsOnFloor(firstStorey->id());
    EXPECT_EQ(wallsInStorey.size(), 3);

    // Verify each wall has correct number of components
    EXPECT_EQ(service.getChildElements(wallNorth->id()).size(), 3);
    EXPECT_EQ(service.getChildElements(wallSouth->id()).size(), 3);
    EXPECT_EQ(service.getChildElements(wallEast->id()).size(), 1);

    // Verify all elements in building (through floor)
    const auto allElementsInBuilding = service.getElementsInBuilding(building->id());
    EXPECT_EQ(allElementsInBuilding.size(), 3);  // Only top-level walls, not sub-components
}

TEST_F(ElementHierarchyTest, ComplexBuildingWithMultipleStoreysAndWalls) {
    // Create a building with multiple storeys, each with walls containing components
    const auto* building = context.createBuilding("Office Complex");

    // Create three storeys
    const auto* groundStorey = context.createFloor("Ground Storey", 0.0);
    const auto* firstStorey = context.createFloor("First Storey", 3.5);
    const auto* secondStorey = context.createFloor("Second Storey", 7.0);

    std::ignore = service.assignFloorToBuilding(groundStorey->id(), building->id());
    std::ignore = service.assignFloorToBuilding(firstStorey->id(), building->id());
    std::ignore = service.assignFloorToBuilding(secondStorey->id(), building->id());

    // Ground Storey - 2 walls
    const auto* groundWall1 = context.createElement("Ground-Wall-1");
    const auto* groundWall2 = context.createElement("Ground-Wall-2");
    std::ignore = service.assignElementToFloor(groundWall1->id(), groundStorey->id());
    std::ignore = service.assignElementToFloor(groundWall2->id(), groundStorey->id());

    // Ground Wall 1 components
    const auto* g1Beam1 = context.createElement("G1-Beam-1");
    const auto* g1Beam2 = context.createElement("G1-Beam-2");
    const auto* g1Panel = context.createElement("G1-Panel");
    std::ignore = service.assignElementToParent(g1Beam1->id(), groundWall1->id());
    std::ignore = service.assignElementToParent(g1Beam2->id(), groundWall1->id());
    std::ignore = service.assignElementToParent(g1Panel->id(), groundWall1->id());

    // Ground Wall 2 components
    const auto* g2Beam = context.createElement("G2-Beam");
    const auto* g2Panel1 = context.createElement("G2-Panel-1");
    const auto* g2Panel2 = context.createElement("G2-Panel-2");
    std::ignore = service.assignElementToParent(g2Beam->id(), groundWall2->id());
    std::ignore = service.assignElementToParent(g2Panel1->id(), groundWall2->id());
    std::ignore = service.assignElementToParent(g2Panel2->id(), groundWall2->id());

    // First Storey - 1 wall
    const auto* firstWall = context.createElement("First-Wall");
    std::ignore = service.assignElementToFloor(firstWall->id(), firstStorey->id());

    const auto* f1Beam = context.createElement("F1-Beam");
    const auto* f1Panel = context.createElement("F1-Panel");
    std::ignore = service.assignElementToParent(f1Beam->id(), firstWall->id());
    std::ignore = service.assignElementToParent(f1Panel->id(), firstWall->id());

    // Second Storey - 1 wall
    const auto* secondWall = context.createElement("Second-Wall");
    std::ignore = service.assignElementToFloor(secondWall->id(), secondStorey->id());

    const auto* s1Beam1 = context.createElement("S1-Beam-1");
    const auto* s1Beam2 = context.createElement("S1-Beam-2");
    const auto* s1Beam3 = context.createElement("S1-Beam-3");
    std::ignore = service.assignElementToParent(s1Beam1->id(), secondWall->id());
    std::ignore = service.assignElementToParent(s1Beam2->id(), secondWall->id());
    std::ignore = service.assignElementToParent(s1Beam3->id(), secondWall->id());

    // Verify building structure
    EXPECT_EQ(service.getFloorsInBuilding(building->id()).size(), 3);

    // Verify each storey
    EXPECT_EQ(service.getElementsOnFloor(groundStorey->id()).size(), 2);
    EXPECT_EQ(service.getElementsOnFloor(firstStorey->id()).size(), 1);
    EXPECT_EQ(service.getElementsOnFloor(secondStorey->id()).size(), 1);

    // Verify wall components
    EXPECT_EQ(service.getChildElements(groundWall1->id()).size(), 3);
    EXPECT_EQ(service.getChildElements(groundWall2->id()).size(), 3);
    EXPECT_EQ(service.getChildElements(firstWall->id()).size(), 2);
    EXPECT_EQ(service.getChildElements(secondWall->id()).size(), 3);

    // Verify total walls in building
    const auto allWalls = service.getElementsInBuilding(building->id());
    EXPECT_EQ(allWalls.size(), 4);  // 2 ground + 1 first + 1 second

    // Verify recursive element retrieval for a wall
    const auto groundWall1AllComponents = service.getAllElementsRecursive(groundWall1->id());
    EXPECT_EQ(groundWall1AllComponents.size(), 4);  // Wall itself + 3 components
}

TEST_F(ElementHierarchyTest, WallWithNestedBeamAssemblies) {
    // Demonstrate that beams can themselves contain sub-elements
    const auto* building = context.createBuilding("Industrial Building");
    const auto* storey = context.createFloor("Main Storey", 0.0);
    std::ignore = service.assignFloorToBuilding(storey->id(), building->id());

    // Create wall
    const auto* wall = context.createElement("Load-Bearing Wall");
    std::ignore = service.assignElementToFloor(wall->id(), storey->id());

    // Create composite beam (beam with sub-components)
    const auto* compositeBeam = context.createElement("Composite Beam");
    std::ignore = service.assignElementToParent(compositeBeam->id(), wall->id());

    // Beam consists of multiple parts
    const auto* steelCore = context.createElement("Steel Core");
    const auto* topFlange = context.createElement("Top Flange");
    const auto* bottomFlange = context.createElement("Bottom Flange");
    const auto* webPlate = context.createElement("Web Plate");

    std::ignore = service.assignElementToParent(steelCore->id(), compositeBeam->id());
    std::ignore = service.assignElementToParent(topFlange->id(), compositeBeam->id());
    std::ignore = service.assignElementToParent(bottomFlange->id(), compositeBeam->id());
    std::ignore = service.assignElementToParent(webPlate->id(), compositeBeam->id());

    // Create panels
    const auto* panel1 = context.createElement("Exterior Panel");
    const auto* panel2 = context.createElement("Interior Panel");
    std::ignore = service.assignElementToParent(panel1->id(), wall->id());
    std::ignore = service.assignElementToParent(panel2->id(), wall->id());

    // Verify hierarchy
    // Wall has 3 direct children: 1 composite beam + 2 panels
    const auto wallDirectChildren = service.getChildElements(wall->id());
    EXPECT_EQ(wallDirectChildren.size(), 3);

    // Composite beam has 4 sub-components
    const auto beamComponents = service.getChildElements(compositeBeam->id());
    EXPECT_EQ(beamComponents.size(), 4);

    // Recursive retrieval from wall includes all nested elements
    const auto allWallComponents = service.getAllElementsRecursive(wall->id());
    EXPECT_EQ(allWallComponents.size(), 8);  // Wall + 3 direct + 4 beam sub-parts

    // Verify parent relationships
    EXPECT_EQ(service.getParentElement(compositeBeam->id())->id(), wall->id());
    EXPECT_EQ(service.getParentElement(steelCore->id())->id(), compositeBeam->id());
    EXPECT_EQ(service.getParentElement(topFlange->id())->id(), compositeBeam->id());
}
