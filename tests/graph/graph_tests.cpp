#include <cwapi3d/graph/Graph.h>
#include <gtest/gtest.h>
#include <string>

using namespace cwapi3d::graph;

class GraphTest : public ::testing::Test {
protected:
    Graph<int> undirectedGraph{false};
    Graph<int> directedGraph{true};
    Graph<std::string> stringGraph{false};
};

// Node operations tests
TEST_F(GraphTest, AddNode) {
    EXPECT_TRUE(undirectedGraph.addNode(1));
    EXPECT_TRUE(undirectedGraph.addNode(2));
    EXPECT_FALSE(undirectedGraph.addNode(1)); // Duplicate
    EXPECT_EQ(undirectedGraph.nodeCount(), 2);
}

TEST_F(GraphTest, RemoveNode) {
    undirectedGraph.addNode(1);
    undirectedGraph.addNode(2);
    undirectedGraph.addEdge(1, 2);

    EXPECT_TRUE(undirectedGraph.removeNode(1));
    EXPECT_FALSE(undirectedGraph.hasNode(1));
    EXPECT_EQ(undirectedGraph.nodeCount(), 1);
    EXPECT_FALSE(undirectedGraph.hasEdge(1, 2));
}

TEST_F(GraphTest, HasNode) {
    undirectedGraph.addNode(1);
    EXPECT_TRUE(undirectedGraph.hasNode(1));
    EXPECT_FALSE(undirectedGraph.hasNode(2));
}

TEST_F(GraphTest, GetNodes) {
    undirectedGraph.addNode(1);
    undirectedGraph.addNode(2);
    undirectedGraph.addNode(3);

    auto nodes = undirectedGraph.getNodes();
    EXPECT_EQ(nodes.size(), 3);
}

// Edge operations tests
TEST_F(GraphTest, AddEdgeUndirected) {
    undirectedGraph.addNode(1);
    undirectedGraph.addNode(2);

    EXPECT_TRUE(undirectedGraph.addEdge(1, 2, 5.0));
    EXPECT_TRUE(undirectedGraph.hasEdge(1, 2));
    EXPECT_TRUE(undirectedGraph.hasEdge(2, 1)); // Undirected
    EXPECT_EQ(undirectedGraph.edgeCount(), 1);
}

TEST_F(GraphTest, AddEdgeDirected) {
    directedGraph.addNode(1);
    directedGraph.addNode(2);

    EXPECT_TRUE(directedGraph.addEdge(1, 2));
    EXPECT_TRUE(directedGraph.hasEdge(1, 2));
    EXPECT_FALSE(directedGraph.hasEdge(2, 1)); // Directed
    EXPECT_EQ(directedGraph.edgeCount(), 1);
}

TEST_F(GraphTest, RemoveEdge) {
    undirectedGraph.addNode(1);
    undirectedGraph.addNode(2);
    undirectedGraph.addEdge(1, 2);

    EXPECT_TRUE(undirectedGraph.removeEdge(1, 2));
    EXPECT_FALSE(undirectedGraph.hasEdge(1, 2));
    EXPECT_EQ(undirectedGraph.edgeCount(), 0);
}

TEST_F(GraphTest, EdgeWeight) {
    undirectedGraph.addNode(1);
    undirectedGraph.addNode(2);
    undirectedGraph.addEdge(1, 2, 7.5);

    auto weight = undirectedGraph.getEdgeWeight(1, 2);
    ASSERT_TRUE(weight.has_value());
    EXPECT_DOUBLE_EQ(*weight, 7.5);

    EXPECT_TRUE(undirectedGraph.setEdgeWeight(1, 2, 10.0));
    weight = undirectedGraph.getEdgeWeight(1, 2);
    ASSERT_TRUE(weight.has_value());
    EXPECT_DOUBLE_EQ(*weight, 10.0);
}

TEST_F(GraphTest, GetEdges) {
    undirectedGraph.addNode(1);
    undirectedGraph.addNode(2);
    undirectedGraph.addNode(3);
    undirectedGraph.addEdge(1, 2, 1.0);
    undirectedGraph.addEdge(2, 3, 2.0);

    auto edges = undirectedGraph.getEdges();
    EXPECT_EQ(edges.size(), 2);
}

// Neighbor operations tests
TEST_F(GraphTest, GetNeighbors) {
    undirectedGraph.addNode(1);
    undirectedGraph.addNode(2);
    undirectedGraph.addNode(3);
    undirectedGraph.addEdge(1, 2);
    undirectedGraph.addEdge(1, 3);

    auto neighbors = undirectedGraph.getNeighbors(1);
    EXPECT_EQ(neighbors.size(), 2);
}

TEST_F(GraphTest, Degree) {
    directedGraph.addNode(1);
    directedGraph.addNode(2);
    directedGraph.addNode(3);
    directedGraph.addEdge(1, 2);
    directedGraph.addEdge(3, 1);

    EXPECT_EQ(directedGraph.getOutDegree(1), 1);
    EXPECT_EQ(directedGraph.getInDegree(1), 1);
    EXPECT_EQ(directedGraph.getDegree(1), 2);
}

// Traversal tests
TEST_F(GraphTest, BreadthFirstSearch) {
    undirectedGraph.addNode(1);
    undirectedGraph.addNode(2);
    undirectedGraph.addNode(3);
    undirectedGraph.addEdge(1, 2);
    undirectedGraph.addEdge(2, 3);

    std::vector<int> visited;
    undirectedGraph.breadthFirstSearch(1, [&visited](int node) {
        visited.push_back(node);
    });

    EXPECT_EQ(visited.size(), 3);
    EXPECT_EQ(visited[0], 1);
}

TEST_F(GraphTest, DepthFirstSearch) {
    undirectedGraph.addNode(1);
    undirectedGraph.addNode(2);
    undirectedGraph.addNode(3);
    undirectedGraph.addEdge(1, 2);
    undirectedGraph.addEdge(2, 3);

    std::vector<int> visited;
    undirectedGraph.depthFirstSearch(1, [&visited](int node) {
        visited.push_back(node);
    });

    EXPECT_EQ(visited.size(), 3);
    EXPECT_EQ(visited[0], 1);
}

// Graph with node data tests
TEST_F(GraphTest, NodeData) {
    struct Data {
        std::string name;
        int value;
    };

    Graph<int, Data> graph(false);

    graph.addNode(1, Data{"First", 100});
    graph.addNode(2, Data{"Second", 200});

    const auto& data = graph.getNodeData(1);
    EXPECT_EQ(data.name, "First");
    EXPECT_EQ(data.value, 100);

    graph.setNodeData(1, Data{"Updated", 150});
    const auto& updated = graph.getNodeData(1);
    EXPECT_EQ(updated.name, "Updated");
    EXPECT_EQ(updated.value, 150);
}

TEST_F(GraphTest, Clear) {
    undirectedGraph.addNode(1);
    undirectedGraph.addNode(2);
    undirectedGraph.addEdge(1, 2);

    undirectedGraph.clear();

    EXPECT_EQ(undirectedGraph.nodeCount(), 0);
    EXPECT_EQ(undirectedGraph.edgeCount(), 0);
}

TEST_F(GraphTest, StringNodes) {
    stringGraph.addNode("A");
    stringGraph.addNode("B");
    stringGraph.addNode("C");
    stringGraph.addEdge("A", "B", 1.5);
    stringGraph.addEdge("B", "C", 2.5);

    EXPECT_TRUE(stringGraph.hasNode("A"));
    EXPECT_TRUE(stringGraph.hasEdge("A", "B"));

    auto neighbors = stringGraph.getNeighbors("B");
    EXPECT_EQ(neighbors.size(), 2);
}

