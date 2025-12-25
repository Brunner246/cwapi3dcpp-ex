#include <gtest/gtest.h>

#include "cwapi3d/model/BRepGeometry.h"
#include "cwapi3d/model/ComponentRegistry.h"
#include "cwapi3d/model/EntityId.h"
#include "cwapi3d/model/IGeometryComponent.h"
#include "cwapi3d/model/PropertySet.h"

using namespace CwAPI3D::Model;

TEST(ComponentRegistryTest, AddAndGetComponent) {
    ComponentRegistry registry;
    auto entityId = EntityId::create();

    auto geom = std::make_unique<BRepGeometry>();
    registry.addComponent(entityId, std::move(geom));

    EXPECT_TRUE(registry.hasComponent<BRepGeometry>(entityId));

    auto* retrieved = registry.getComponent<BRepGeometry>(entityId);
    EXPECT_NE(retrieved, nullptr);
}

TEST(ComponentRegistryTest, GetNonExistentComponent) {
    ComponentRegistry registry;
    auto entityId = EntityId::create();

    auto* component = registry.getComponent<BRepGeometry>(entityId);
    EXPECT_EQ(component, nullptr);
    EXPECT_FALSE(registry.hasComponent<BRepGeometry>(entityId));
}

TEST(ComponentRegistryTest, RemoveComponent) {
    ComponentRegistry registry;
    auto entityId = EntityId::create();

    auto geom = std::make_unique<BRepGeometry>();
    registry.addComponent(entityId, std::move(geom));

    EXPECT_TRUE(registry.hasComponent<BRepGeometry>(entityId));

    bool removed = registry.removeComponent<BRepGeometry>(entityId);
    EXPECT_TRUE(removed);
    EXPECT_FALSE(registry.hasComponent<BRepGeometry>(entityId));
}

TEST(ComponentRegistryTest, RemoveNonExistentComponent) {
    ComponentRegistry registry;
    auto entityId = EntityId::create();

    bool removed = registry.removeComponent<BRepGeometry>(entityId);
    EXPECT_FALSE(removed);
}

TEST(ComponentRegistryTest, MultipleComponentsPerEntity) {
    ComponentRegistry registry;
    auto entityId = EntityId::create();

    auto geom = std::make_unique<BRepGeometry>();
    auto props = std::make_unique<PropertySet>("Properties");

    registry.addComponent(entityId, std::move(geom));
    registry.addComponent(entityId, std::move(props));

    EXPECT_TRUE(registry.hasComponent<BRepGeometry>(entityId));
    EXPECT_TRUE(registry.hasComponent<PropertySet>(entityId));
    EXPECT_EQ(registry.getComponentCount(entityId), 2);
}

TEST(ComponentRegistryTest, ReplaceComponent) {
    ComponentRegistry registry;
    auto entityId = EntityId::create();

    auto geom1 = std::make_unique<BRepGeometry>();
    geom1->addVertex({{0, 0, 0}});

    registry.addComponent(entityId, std::move(geom1));

    auto* first = registry.getComponent<BRepGeometry>(entityId);
    EXPECT_EQ(first->vertices().size(), 1);

    // Replace with new geometry
    auto geom2 = std::make_unique<BRepGeometry>();
    geom2->addVertex({{1, 1, 1}});
    geom2->addVertex({{2, 2, 2}});

    registry.addComponent(entityId, std::move(geom2));

    auto* second = registry.getComponent<BRepGeometry>(entityId);
    EXPECT_EQ(second->vertices().size(), 2);
}

TEST(ComponentRegistryTest, RemoveAllComponents) {
    ComponentRegistry registry;
    auto entityId = EntityId::create();

    registry.addComponent(entityId, std::make_unique<BRepGeometry>());
    registry.addComponent(entityId, std::make_unique<PropertySet>());

    EXPECT_EQ(registry.getComponentCount(entityId), 2);

    registry.removeAllComponents(entityId);

    EXPECT_EQ(registry.getComponentCount(entityId), 0);
    EXPECT_FALSE(registry.hasComponent<BRepGeometry>(entityId));
    EXPECT_FALSE(registry.hasComponent<PropertySet>(entityId));
}

TEST(ComponentRegistryTest, GetEntitiesWithComponent) {
    ComponentRegistry registry;

    auto id1 = EntityId::create();
    auto id2 = EntityId::create();
    auto id3 = EntityId::create();

    registry.addComponent(id1, std::make_unique<BRepGeometry>());
    registry.addComponent(id2, std::make_unique<BRepGeometry>());
    registry.addComponent(id3, std::make_unique<PropertySet>());

    auto entities = registry.getEntitiesWithComponent<BRepGeometry>();
    EXPECT_EQ(entities.size(), 2);

    auto propEntities = registry.getEntitiesWithComponent<PropertySet>();
    EXPECT_EQ(propEntities.size(), 1);
}

TEST(ComponentRegistryTest, EntityCount) {
    ComponentRegistry registry;

    EXPECT_EQ(registry.getEntityCount(), 0);

    auto id1 = EntityId::create();
    auto id2 = EntityId::create();

    registry.addComponent(id1, std::make_unique<BRepGeometry>());
    EXPECT_EQ(registry.getEntityCount(), 1);

    registry.addComponent(id2, std::make_unique<PropertySet>());
    EXPECT_EQ(registry.getEntityCount(), 2);

    registry.removeAllComponents(id1);
    EXPECT_EQ(registry.getEntityCount(), 1);
}
