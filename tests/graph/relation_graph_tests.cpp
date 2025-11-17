#include <cwapi3d/graph/RelationGraph.h>
#include <gtest/gtest.h>
#include <string>

using namespace cwapi3d::graph;

class RelationGraphTest : public ::testing::Test {
protected:
    RelationGraph<std::string> directedRelations{true};
    RelationGraph<std::string> undirectedRelations{false};
};

TEST_F(RelationGraphTest, AddElement) {
    EXPECT_TRUE(directedRelations.addElement("A"));
    EXPECT_TRUE(directedRelations.addElement("B"));
    EXPECT_FALSE(directedRelations.addElement("A")); // Duplicate
    EXPECT_EQ(directedRelations.elementCount(), 2);
}

TEST_F(RelationGraphTest, RemoveElement) {
    directedRelations.addElement("A");
    directedRelations.addElement("B");
    directedRelations.addRelation("A", "B");

    EXPECT_TRUE(directedRelations.removeElement("A"));
    EXPECT_FALSE(directedRelations.hasElement("A"));
    EXPECT_EQ(directedRelations.elementCount(), 1);
}

TEST_F(RelationGraphTest, AddRelationAutoCreate) {
    // Relations should auto-create elements
    EXPECT_TRUE(directedRelations.addRelation("A", "B"));
    EXPECT_TRUE(directedRelations.hasElement("A"));
    EXPECT_TRUE(directedRelations.hasElement("B"));
    EXPECT_TRUE(directedRelations.hasRelation("A", "B"));
}

TEST_F(RelationGraphTest, DirectedRelations) {
    directedRelations.addRelation("Parent", "Child");

    EXPECT_TRUE(directedRelations.hasRelation("Parent", "Child"));
    EXPECT_FALSE(directedRelations.hasRelation("Child", "Parent"));
}

TEST_F(RelationGraphTest, UndirectedRelations) {
    undirectedRelations.addRelation("Friend1", "Friend2");

    EXPECT_TRUE(undirectedRelations.hasRelation("Friend1", "Friend2"));
    EXPECT_TRUE(undirectedRelations.hasRelation("Friend2", "Friend1"));
}

TEST_F(RelationGraphTest, GetRelatedElements) {
    directedRelations.addRelation("A", "B");
    directedRelations.addRelation("A", "C");
    directedRelations.addRelation("A", "D");

    auto related = directedRelations.getRelatedElements("A");
    EXPECT_EQ(related.size(), 3);
}

TEST_F(RelationGraphTest, GetIncomingRelations) {
    directedRelations.addRelation("A", "Target");
    directedRelations.addRelation("B", "Target");
    directedRelations.addRelation("C", "Target");

    auto incoming = directedRelations.getIncomingRelations("Target");
    EXPECT_EQ(incoming.size(), 3);
}

TEST_F(RelationGraphTest, RemoveRelation) {
    directedRelations.addRelation("A", "B");
    EXPECT_TRUE(directedRelations.hasRelation("A", "B"));

    EXPECT_TRUE(directedRelations.removeRelation("A", "B"));
    EXPECT_FALSE(directedRelations.hasRelation("A", "B"));
}

TEST_F(RelationGraphTest, RelationCount) {
    directedRelations.addRelation("A", "B");
    directedRelations.addRelation("B", "C");
    directedRelations.addRelation("C", "D");

    EXPECT_EQ(directedRelations.relationCount(), 3);
}

TEST_F(RelationGraphTest, TraverseBFS) {
    directedRelations.addRelation("A", "B");
    directedRelations.addRelation("B", "C");
    directedRelations.addRelation("C", "D");

    std::vector<std::string> visited;
    directedRelations.traverse("A", [&visited](const std::string& elem) {
        visited.push_back(elem);
    }, true);

    EXPECT_EQ(visited.size(), 4);
    EXPECT_EQ(visited[0], "A");
}

TEST_F(RelationGraphTest, TraverseDFS) {
    directedRelations.addRelation("A", "B");
    directedRelations.addRelation("B", "C");
    directedRelations.addRelation("C", "D");

    std::vector<std::string> visited;
    directedRelations.traverse("A", [&visited](const std::string& elem) {
        visited.push_back(elem);
    }, false);

    EXPECT_EQ(visited.size(), 4);
    EXPECT_EQ(visited[0], "A");
}

TEST_F(RelationGraphTest, Clear) {
    directedRelations.addRelation("A", "B");
    directedRelations.addRelation("B", "C");

    directedRelations.clear();

    EXPECT_EQ(directedRelations.elementCount(), 0);
    EXPECT_EQ(directedRelations.relationCount(), 0);
}

TEST_F(RelationGraphTest, DependencyExample) {
    RelationGraph<std::string> deps(true);

    // Build a dependency graph
    deps.addRelation("App", "Database");
    deps.addRelation("App", "WebServer");
    deps.addRelation("Database", "Storage");
    deps.addRelation("WebServer", "Network");

    // Check App's dependencies
    auto appDeps = deps.getRelatedElements("App");
    EXPECT_EQ(appDeps.size(), 2);

    // Check what depends on Network
    auto networkDependents = deps.getIncomingRelations("Network");
    EXPECT_EQ(networkDependents.size(), 1);
}

// Test with relation data
TEST_F(RelationGraphTest, RelationWithData) {
    struct RelationInfo {
        std::string type;
        double strength;
    };

    RelationGraph<int, RelationInfo> socialGraph(false);

    socialGraph.addRelation(1, 2, RelationInfo{"friend", 0.8});
    socialGraph.addRelation(2, 3, RelationInfo{"colleague", 0.6});

    auto relationData = socialGraph.getRelationData(1, 2);
    ASSERT_TRUE(relationData.has_value());
    EXPECT_EQ(relationData->type, "friend");
    EXPECT_DOUBLE_EQ(relationData->strength, 0.8);
}

