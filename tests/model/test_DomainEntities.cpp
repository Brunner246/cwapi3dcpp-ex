#include <gtest/gtest.h>

#include "cwapi3d/model/Building.h"
#include "cwapi3d/model/BuildingStorey.h"
#include "cwapi3d/model/Element.h"

using namespace CwAPI3D::Model;

TEST(DomainEntitiesTest, BuildingCreation) {
    Building building("Test Building");

    EXPECT_EQ(building.name(), "Test Building");
    EXPECT_TRUE(building.id().isValid());
    EXPECT_FALSE(building.address().has_value());
}

TEST(DomainEntitiesTest, BuildingAddress) {
    Building building("Office");

    building.setAddress("123 Main St");
    EXPECT_TRUE(building.address().has_value());
    EXPECT_EQ(building.address().value(), "123 Main St");

    building.clearAddress();
    EXPECT_FALSE(building.address().has_value());
}

TEST(DomainEntitiesTest, FloorCreation) {
    BuildingStorey floor("Ground Floor", 0.0);

    EXPECT_EQ(floor.name(), "Ground Floor");
    EXPECT_TRUE(floor.id().isValid());
    EXPECT_TRUE(floor.elevation().has_value());
    EXPECT_DOUBLE_EQ(floor.elevation().value(), 0.0);
}

TEST(DomainEntitiesTest, FloorElevation) {
    BuildingStorey floor("First Floor");

    EXPECT_FALSE(floor.elevation().has_value());

    floor.setElevation(3.5);
    EXPECT_TRUE(floor.elevation().has_value());
    EXPECT_DOUBLE_EQ(floor.elevation().value(), 3.5);

    floor.clearElevation();
    EXPECT_FALSE(floor.elevation().has_value());
}

TEST(DomainEntitiesTest, ElementCreation) {
    Element element("Wall-001");

    EXPECT_EQ(element.name(), "Wall-001");
    EXPECT_TRUE(element.id().isValid());
    EXPECT_FALSE(element.description().has_value());
}

TEST(DomainEntitiesTest, ElementDescription) {
    Element element("Column");

    element.setDescription("Structural column");
    EXPECT_TRUE(element.description().has_value());
    EXPECT_EQ(element.description().value(), "Structural column");

    element.clearDescription();
    EXPECT_FALSE(element.description().has_value());
}

TEST(DomainEntitiesTest, UniqueIdentities) {
    Building b1("B1");
    Building b2("B2");
    BuildingStorey f1("F1");
    Element e1("E1");

    EXPECT_NE(b1.id(), b2.id());
    EXPECT_NE(b1.id(), f1.id());
    EXPECT_NE(b1.id(), e1.id());
    EXPECT_NE(f1.id(), e1.id());
}
