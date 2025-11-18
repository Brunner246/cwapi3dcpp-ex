#include <gtest/gtest.h>
#include "cwapi3d/composite/Component.h"
#include "cwapi3d/composite/ComponentVisitor.h"

using namespace cwapi3d::composite;

TEST(LeafTest, CreationAndProperties) {
    const auto leaf = std::make_unique<Leaf>("TestLeaf");
    
    EXPECT_EQ(leaf->name(), "TestLeaf");
    EXPECT_FALSE(leaf->isComposite());
    EXPECT_EQ(leaf->parent(), nullptr);
}

TEST(CompositeTest, CreationAndProperties) {
    const auto composite = std::make_unique<Composite>("TestComposite");
    
    EXPECT_EQ(composite->name(), "TestComposite");
    EXPECT_TRUE(composite->isComposite());
    EXPECT_EQ(composite->childCount(), 0);
}

TEST(CompositeTest, AddChildrenCount) {
    const auto root = std::make_unique<Composite>("Root");
    auto child1 = std::make_unique<Leaf>("Child1");
    auto child2 = std::make_unique<Leaf>("Child2");

    // Component* child1Ptr = child1.get();
    // Component* child2Ptr = child2.get();

    root->addChild(std::move(child1));
    root->addChild(std::move(child2));

    EXPECT_EQ(root->childCount(), 2);
}

TEST(CompositeTest, AddChildrenParentRelationships) {
    const auto root = std::make_unique<Composite>("Root");
    auto child1 = std::make_unique<Leaf>("Child1");
    auto child2 = std::make_unique<Leaf>("Child2");

    const Component* child1Ptr = child1.get();
    const Component* child2Ptr = child2.get();

    root->addChild(std::move(child1));
    root->addChild(std::move(child2));

    EXPECT_EQ(child1Ptr->parent(), root.get());
    EXPECT_EQ(child2Ptr->parent(), root.get());
}

TEST(CompositeTest, AddChildrenGetByIndex) {
    const auto root = std::make_unique<Composite>("Root");
    auto child1 = std::make_unique<Leaf>("Child1");
    auto child2 = std::make_unique<Leaf>("Child2");
    
    Component* child1Ptr = child1.get();
    Component* child2Ptr = child2.get();
    
    root->addChild(std::move(child1));
    root->addChild(std::move(child2));
    
    EXPECT_EQ(root->getChild(0), child1Ptr);
    EXPECT_EQ(root->getChild(1), child2Ptr);
}

TEST(CompositeTest, RemoveChildByPointer) {
    const auto root = std::make_unique<Composite>("Root");
    auto child = std::make_unique<Leaf>("Child");
    
    Component* childPtr = child.get();
    root->addChild(std::move(child));
    
    EXPECT_EQ(root->childCount(), 1);

    const auto removed = root->removeChild(childPtr);
    EXPECT_EQ(root->childCount(), 0);
    EXPECT_NE(removed, nullptr);
    EXPECT_EQ(removed->parent(), nullptr);
    EXPECT_EQ(removed->name(), "Child");
}

TEST(CompositeTest, RemoveChildByName) {
    const auto root = std::make_unique<Composite>("Root");
    root->addChild(std::make_unique<Leaf>("Child1"));
    root->addChild(std::make_unique<Leaf>("Child2"));

    const auto removed = root->removeChild("Child1");
    
    EXPECT_NE(removed, nullptr);
    EXPECT_EQ(removed->name(), "Child1");
    EXPECT_EQ(root->childCount(), 1);
}

TEST(CompositeTest, HierarchyRootChildren) {
    const auto root = std::make_unique<Composite>("Building");

    auto floor1 = std::make_unique<Composite>("Floor1");
    floor1->addChild(std::make_unique<Leaf>("Room101"));
    floor1->addChild(std::make_unique<Leaf>("Room102"));

    auto floor2 = std::make_unique<Composite>("Floor2");
    floor2->addChild(std::make_unique<Leaf>("Room201"));

    root->addChild(std::move(floor1));
    root->addChild(std::move(floor2));

    EXPECT_EQ(root->childCount(), 2);
}

TEST(CompositeTest, HierarchyNestedComposite) {
    const auto root = std::make_unique<Composite>("Building");
    
    auto floor1 = std::make_unique<Composite>("Floor1");
    floor1->addChild(std::make_unique<Leaf>("Room101"));
    floor1->addChild(std::make_unique<Leaf>("Room102"));
    
    auto floor2 = std::make_unique<Composite>("Floor2");
    floor2->addChild(std::make_unique<Leaf>("Room201"));
    
    root->addChild(std::move(floor1));
    root->addChild(std::move(floor2));

    const Composite* floor1Comp = dynamic_cast<Composite*>(root->getChild(0));
    EXPECT_EQ(floor1Comp->childCount(), 2);
}

TEST(CompositeTest, TraversalCountAll) {
    const auto root = std::make_unique<Composite>("Root");
    root->addChild(std::make_unique<Leaf>("Child1"));
    root->addChild(std::make_unique<Leaf>("Child2"));
    
    int count = 0;
    root->traverse([&count](const Component& comp) {
        std::ignore = comp;
        ++count;
    });
    EXPECT_EQ(count, 3); // Root + 2 children
}

TEST(CompositeTest, TraversalCollectNames) {
    const auto root = std::make_unique<Composite>("Root");
    root->addChild(std::make_unique<Leaf>("Child1"));
    root->addChild(std::make_unique<Leaf>("Child2"));

    std::vector<std::string> names;
    root->traverse([&names](const Component& comp) {
        names.push_back(comp.name());
    });
    EXPECT_EQ(names.size(), 3);
    EXPECT_EQ(names[0], "Root");
}

TEST(CompositeTest, FindExistingChild) {
    const auto root = std::make_unique<Composite>("Root");
    root->addChild(std::make_unique<Leaf>("Child1"));
    root->addChild(std::make_unique<Leaf>("Child2"));
    
    Component* found = root->findChild("Child1");
    EXPECT_NE(found, nullptr);
    EXPECT_EQ(found->name(), "Child1");
}

TEST(CompositeTest, FindNonExistentChild) {
    const auto root = std::make_unique<Composite>("Root");
    root->addChild(std::make_unique<Leaf>("Child1"));
    root->addChild(std::make_unique<Leaf>("Child2"));

    Component* notFound = root->findChild("NonExistent");
    EXPECT_EQ(notFound, nullptr);
}

TEST(CompositeTest, ClearChildren) {
    const auto root = std::make_unique<Composite>("Root");
    root->addChild(std::make_unique<Leaf>("Child1"));
    root->addChild(std::make_unique<Leaf>("Child2"));
    
    EXPECT_EQ(root->childCount(), 2);

    root->clearChildren();
    
    EXPECT_EQ(root->childCount(), 0);
}

TEST(CompositeTest, Iteration) {
    const auto root = std::make_unique<Composite>("Root");
    root->addChild(std::make_unique<Leaf>("Child1"));
    root->addChild(std::make_unique<Leaf>("Child2"));
    root->addChild(std::make_unique<Leaf>("Child3"));
    
    int count = 0;
    for (const auto& child : *root) {
        std::ignore = child;
        ++count;
    }
    EXPECT_EQ(count, 3);
}
