#include <gtest/gtest.h>

#include "cwapi3d/model/EntityId.h"

using namespace CwAPI3D::Model;

TEST(EntityIdTest, CreateUniqueIds) {
    auto id1 = EntityId::create();
    auto id2 = EntityId::create();

    EXPECT_TRUE(id1.isValid());
    EXPECT_TRUE(id2.isValid());
    EXPECT_NE(id1, id2);
}

TEST(EntityIdTest, DefaultConstructedIsInvalid) {
    EntityId id;
    EXPECT_FALSE(id.isValid());
}

TEST(EntityIdTest, Equality) {
    auto id1 = EntityId::create();
    auto id2 = EntityId(id1.value());

    EXPECT_EQ(id1, id2);
}

TEST(EntityIdTest, HashSupport) {
    auto id1 = EntityId::create();
    auto id2 = EntityId::create();

    std::hash<EntityId> hasher;
    EXPECT_NE(hasher(id1), hasher(id2));
}

TEST(EntityIdTest, Ordering) {
    auto id1 = EntityId::create();
    auto id2 = EntityId::create();

    // Since IDs are sequential, id1 should be less than id2
    EXPECT_LT(id1, id2);
}

TEST(EntityIdTest, ToString) {
    auto id = EntityId::create();
    std::string str = id.toString();

    EXPECT_FALSE(str.empty());
    EXPECT_NE(str.find("EntityId"), std::string::npos);
}
