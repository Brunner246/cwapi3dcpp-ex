#include <gtest/gtest.h>

#include "cwapi3d/composite/Component.h"
#include "cwapi3d/composite/ComponentVisitor.h"

using namespace CwAPI3D::Composite;

// ===== Generic Component Tests with int =====

TEST(GenericComponentTest, LeafCreation) {
    const auto leaf = std::make_unique<LeafNode<int>>("TestLeaf", 42);

    EXPECT_EQ(leaf->name(), "TestLeaf");
    EXPECT_TRUE(leaf->hasData());
    EXPECT_EQ(leaf->data(), 42);
    EXPECT_FALSE(leaf->isComposite());
    EXPECT_EQ(leaf->parent(), nullptr);
}

TEST(GenericComponentTest, LeafWithoutData) {
    const auto leaf = std::make_unique<LeafNode<int>>("TestLeaf");

    EXPECT_EQ(leaf->name(), "TestLeaf");
    EXPECT_FALSE(leaf->hasData());
    EXPECT_THROW(std::ignore = leaf->data(), std::runtime_error);
}

TEST(GenericComponentTest, CompositeCreation) {
    const auto composite = std::make_unique<CompositeNode<int>>("TestComposite");

    EXPECT_EQ(composite->name(), "TestComposite");
    EXPECT_TRUE(composite->isComposite());
    EXPECT_EQ(composite->childCount(), 0);
}

TEST(GenericComponentTest, AddChild) {
    const auto composite = std::make_unique<CompositeNode<int>>("Parent");
    auto leaf = std::make_unique<LeafNode<int>>("Child", 10);
    const auto* leafPtr = leaf.get();

    composite->addChild(std::move(leaf));

    EXPECT_EQ(composite->childCount(), 1);
    EXPECT_EQ(leafPtr->parent(), composite.get());
}

TEST(GenericComponentTest, AddMultipleChildren) {
    const auto composite = std::make_unique<CompositeNode<int>>("Parent");

    composite->addChild(std::make_unique<LeafNode<int>>("Child1", 1));
    composite->addChild(std::make_unique<LeafNode<int>>("Child2", 2));
    composite->addChild(std::make_unique<LeafNode<int>>("Child3", 3));

    EXPECT_EQ(composite->childCount(), 3);
}

TEST(GenericComponentTest, GetChild) {
    const auto composite = std::make_unique<CompositeNode<int>>("Parent");

    composite->addChild(std::make_unique<LeafNode<int>>("Child1", 1));
    composite->addChild(std::make_unique<LeafNode<int>>("Child2", 2));

    auto* child1 = composite->getChild(0);
    auto* child2 = composite->getChild(1);

    EXPECT_EQ(child1->name(), "Child1");
    EXPECT_EQ(child2->name(), "Child2");
    EXPECT_EQ(child1->data(), 1);
    EXPECT_EQ(child2->data(), 2);
}

TEST(GenericComponentTest, GetChildOutOfRange) {
    auto composite = std::make_unique<CompositeNode<int>>("Parent");
    composite->addChild(std::make_unique<LeafNode<int>>("Child1", 1));

    EXPECT_THROW(std::ignore = composite->getChild(1), std::out_of_range);
    EXPECT_THROW(std::ignore = composite->getChild(100), std::out_of_range);
}

TEST(GenericComponentTest, FindChild) {
    const auto composite = std::make_unique<CompositeNode<int>>("Parent");

    composite->addChild(std::make_unique<LeafNode<int>>("Child1", 1));
    composite->addChild(std::make_unique<LeafNode<int>>("Child2", 2));

    auto* found = composite->findChild("Child2");
    EXPECT_NE(found, nullptr);
    EXPECT_EQ(found->name(), "Child2");

    auto* notFound = composite->findChild("NonExistent");
    EXPECT_EQ(notFound, nullptr);
}

TEST(GenericComponentTest, RemoveChildByPointer) {
    const auto composite = std::make_unique<CompositeNode<int>>("Parent");

    auto leaf = std::make_unique<LeafNode<int>>("Child", 42);
    auto* leafPtr = leaf.get();
    composite->addChild(std::move(leaf));

    EXPECT_EQ(composite->childCount(), 1);

    const auto removed = composite->removeChild(leafPtr);

    EXPECT_EQ(composite->childCount(), 0);
    EXPECT_NE(removed, nullptr);
    EXPECT_EQ(removed->name(), "Child");
    EXPECT_EQ(removed->parent(), nullptr);
}

TEST(GenericComponentTest, RemoveChildByName) {
    const auto composite = std::make_unique<CompositeNode<int>>("Parent");

    composite->addChild(std::make_unique<LeafNode<int>>("Child1", 1));
    composite->addChild(std::make_unique<LeafNode<int>>("Child2", 2));

    const auto removed = composite->removeChild("Child1");

    EXPECT_EQ(composite->childCount(), 1);
    EXPECT_NE(removed, nullptr);
    EXPECT_EQ(removed->name(), "Child1");
}

TEST(GenericComponentTest, ClearChildren) {
    const auto composite = std::make_unique<CompositeNode<int>>("Parent");

    composite->addChild(std::make_unique<LeafNode<int>>("Child1", 1));
    composite->addChild(std::make_unique<LeafNode<int>>("Child2", 2));
    composite->addChild(std::make_unique<LeafNode<int>>("Child3", 3));

    EXPECT_EQ(composite->childCount(), 3);

    composite->clearChildren();

    EXPECT_EQ(composite->childCount(), 0);
}

TEST(GenericComponentTest, Iteration) {
    const auto composite = std::make_unique<CompositeNode<int>>("Parent");

    composite->addChild(std::make_unique<LeafNode<int>>("Child1", 10));
    composite->addChild(std::make_unique<LeafNode<int>>("Child2", 20));
    composite->addChild(std::make_unique<LeafNode<int>>("Child3", 30));

    int sum = 0;
    for (const auto& child : *composite) {
        if (child->hasData()) {
            sum += child->data();
        }
    }

    EXPECT_EQ(sum, 60);
}

TEST(GenericComponentTest, TraversalMutable) {
    const auto root = std::make_unique<CompositeNode<int>>("Root");

    auto composite1 = std::make_unique<CompositeNode<int>>("Composite1");
    composite1->addChild(std::make_unique<LeafNode<int>>("Leaf1", 1));
    composite1->addChild(std::make_unique<LeafNode<int>>("Leaf2", 2));

    root->addChild(std::move(composite1));
    root->addChild(std::make_unique<LeafNode<int>>("Leaf3", 3));

    int count = 0;
    root->traverse([&count]([[maybe_unused]] ComponentBase<int>& comp) { count++; });

    EXPECT_EQ(count, 5);  // Root + Composite1 + 3 Leafs
}

TEST(GenericComponentTest, TraversalConst) {
    const auto root = std::make_unique<CompositeNode<int>>("Root");

    auto composite1 = std::make_unique<CompositeNode<int>>("Composite1");
    composite1->addChild(std::make_unique<LeafNode<int>>("Leaf1", 1));
    composite1->addChild(std::make_unique<LeafNode<int>>("Leaf2", 2));

    root->addChild(std::move(composite1));

    int sum = 0;
    root->traverse([&sum](const ComponentBase<int>& comp) {
        if (comp.hasData()) {
            sum += comp.data();
        }
    });

    EXPECT_EQ(sum, 3);  // 1 + 2
}

TEST(GenericComponentTest, DataModification) {
    const auto leaf = std::make_unique<LeafNode<int>>("TestLeaf", 10);

    EXPECT_EQ(leaf->data(), 10);
}

TEST(GenericComponentTest, DataClear) {
    const auto leaf = std::make_unique<LeafNode<int>>("TestLeaf", 42);

    EXPECT_TRUE(leaf->hasData());

    leaf->clearData();

    EXPECT_FALSE(leaf->hasData());
    EXPECT_THROW(std::ignore = leaf->data(), std::runtime_error);
}

TEST(GenericComponentTest, NameModification) {
    const auto leaf = std::make_unique<LeafNode<int>>("OldName", 42);

    EXPECT_EQ(leaf->name(), "OldName");

    leaf->setName("NewName");

    EXPECT_EQ(leaf->name(), "NewName");
}

TEST(GenericComponentTest, PreventNullChild) {
    const auto composite = std::make_unique<CompositeNode<int>>("Test");

    EXPECT_THROW(composite->addChild(nullptr), std::invalid_argument);
}

// ===== Tests with different data types =====

TEST(GenericComponentTest, StringDataType) {
    const auto leaf = std::make_unique<LeafNode<std::string>>("StringLeaf", "Hello, World!");

    EXPECT_EQ(leaf->data(), "Hello, World!");
}

TEST(GenericComponentTest, DoubleDataType) {
    const auto composite = std::make_unique<CompositeNode<double>>("DoubleComposite");

    composite->addChild(std::make_unique<LeafNode<double>>("Value1", 3.14));
    composite->addChild(std::make_unique<LeafNode<double>>("Value2", 2.71));

    double sum = 0.0;
    for (const auto& child : *composite) {
        if (child->hasData()) {
            sum += child->data();
        }
    }

    EXPECT_NEAR(sum, 5.85, 1e-9);
}

struct CustomData {
    int id;
    std::string name;

    bool operator==(const CustomData& other) const { return id == other.id && name == other.name; }
};

TEST(GenericComponentTest, CustomStructDataType) {
    CustomData data{42, "TestData"};
    const auto leaf = std::make_unique<LeafNode<CustomData>>("CustomLeaf", data);

    EXPECT_EQ(leaf->data().id, 42);
    EXPECT_EQ(leaf->data().name, "TestData");
}

// ===== Visitor Tests =====

TEST(GenericComponentTest, PrintVisitor) {
    const auto root = std::make_unique<CompositeNode<int>>("Root", 0);
    root->addChild(std::make_unique<LeafNode<int>>("Leaf1", 10));
    root->addChild(std::make_unique<LeafNode<int>>("Leaf2", 20));

    std::ostringstream oss;
    const PrintVisitorBase<int> printer(oss);
    root->accept(printer);

    const std::string output = oss.str();
    EXPECT_TRUE(output.find("Root") != std::string::npos);
    EXPECT_TRUE(output.find("Leaf1") != std::string::npos);
    EXPECT_TRUE(output.find("Leaf2") != std::string::npos);
}
