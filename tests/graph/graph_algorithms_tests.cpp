#include <cwapi3d/graph/Graph.h>
#include <cwapi3d/graph/GraphAlgorithms.h>
#include <gtest/gtest.h>

using namespace cwapi3d::graph;

class GraphAlgorithmsTest : public ::testing::Test {
protected:
    Graph<int> undirectedGraph{false};
    Graph<int> directedGraph{true};
};

// Dijkstra's algorithm tests
TEST_F(GraphAlgorithmsTest, Dijkstra) {
    undirectedGraph.addNode(1);
    undirectedGraph.addNode(2);
    undirectedGraph.addNode(3);
    undirectedGraph.addNode(4);

    undirectedGraph.addEdge(1, 2, 1.0);
    undirectedGraph.addEdge(2, 3, 2.0);
    undirectedGraph.addEdge(1, 4, 5.0);
    undirectedGraph.addEdge(3, 4, 1.0);

    auto distances = GraphAlgorithms<int>::dijkstra(undirectedGraph, 1);

    EXPECT_DOUBLE_EQ(distances[1], 0.0);
    EXPECT_DOUBLE_EQ(distances[2], 1.0);
    EXPECT_DOUBLE_EQ(distances[3], 3.0);
    EXPECT_DOUBLE_EQ(distances[4], 4.0);
}

TEST_F(GraphAlgorithmsTest, ShortestPath) {
    undirectedGraph.addNode(1);
    undirectedGraph.addNode(2);
    undirectedGraph.addNode(3);
    undirectedGraph.addNode(4);

    undirectedGraph.addEdge(1, 2, 1.0);
    undirectedGraph.addEdge(2, 3, 2.0);
    undirectedGraph.addEdge(1, 4, 5.0);
    undirectedGraph.addEdge(3, 4, 1.0);

    auto path = GraphAlgorithms<int>::shortestPath(undirectedGraph, 1, 4);

    ASSERT_FALSE(path.empty());
    EXPECT_EQ(path.front(), 1);
    EXPECT_EQ(path.back(), 4);
    // Path should be: 1 -> 2 -> 3 -> 4
    EXPECT_EQ(path.size(), 4);
}

TEST_F(GraphAlgorithmsTest, ShortestPathNoPath) {
    undirectedGraph.addNode(1);
    undirectedGraph.addNode(2);
    undirectedGraph.addNode(3);

    undirectedGraph.addEdge(1, 2, 1.0);
    // 3 is disconnected

    auto path = GraphAlgorithms<int>::shortestPath(undirectedGraph, 1, 3);
    EXPECT_TRUE(path.empty());
}

// Connectivity tests
TEST_F(GraphAlgorithmsTest, IsConnectedTrue) {
    undirectedGraph.addNode(1);
    undirectedGraph.addNode(2);
    undirectedGraph.addNode(3);
    undirectedGraph.addEdge(1, 2);
    undirectedGraph.addEdge(2, 3);

    EXPECT_TRUE(GraphAlgorithms<int>::isConnected(undirectedGraph));
}

TEST_F(GraphAlgorithmsTest, IsConnectedFalse) {
    undirectedGraph.addNode(1);
    undirectedGraph.addNode(2);
    undirectedGraph.addNode(3);
    undirectedGraph.addEdge(1, 2);
    // 3 is disconnected

    EXPECT_FALSE(GraphAlgorithms<int>::isConnected(undirectedGraph));
}

TEST_F(GraphAlgorithmsTest, FindConnectedComponents) {
    undirectedGraph.addNode(1);
    undirectedGraph.addNode(2);
    undirectedGraph.addNode(3);
    undirectedGraph.addNode(4);
    undirectedGraph.addNode(5);

    undirectedGraph.addEdge(1, 2);
    undirectedGraph.addEdge(3, 4);
    // 5 is isolated

    auto components = GraphAlgorithms<int>::findConnectedComponents(undirectedGraph);

    EXPECT_EQ(components.size(), 3);
}

// Cycle detection tests
TEST_F(GraphAlgorithmsTest, HasCycleUndirected) {
    undirectedGraph.addNode(1);
    undirectedGraph.addNode(2);
    undirectedGraph.addNode(3);
    undirectedGraph.addEdge(1, 2);
    undirectedGraph.addEdge(2, 3);
    undirectedGraph.addEdge(3, 1);

    EXPECT_TRUE(GraphAlgorithms<int>::hasCycle(undirectedGraph));
}

TEST_F(GraphAlgorithmsTest, NoCycleUndirected) {
    undirectedGraph.addNode(1);
    undirectedGraph.addNode(2);
    undirectedGraph.addNode(3);
    undirectedGraph.addEdge(1, 2);
    undirectedGraph.addEdge(2, 3);

    EXPECT_FALSE(GraphAlgorithms<int>::hasCycle(undirectedGraph));
}

TEST_F(GraphAlgorithmsTest, HasCycleDirected) {
    directedGraph.addNode(1);
    directedGraph.addNode(2);
    directedGraph.addNode(3);
    directedGraph.addEdge(1, 2);
    directedGraph.addEdge(2, 3);
    directedGraph.addEdge(3, 1);

    EXPECT_TRUE(GraphAlgorithms<int>::hasCycle(directedGraph));
}

TEST_F(GraphAlgorithmsTest, NoCycleDirected) {
    directedGraph.addNode(1);
    directedGraph.addNode(2);
    directedGraph.addNode(3);
    directedGraph.addEdge(1, 2);
    directedGraph.addEdge(2, 3);

    EXPECT_FALSE(GraphAlgorithms<int>::hasCycle(directedGraph));
}

// Topological sort tests
TEST_F(GraphAlgorithmsTest, TopologicalSort) {
    directedGraph.addNode(5);
    directedGraph.addNode(4);
    directedGraph.addNode(2);
    directedGraph.addNode(3);
    directedGraph.addNode(1);
    directedGraph.addNode(0);

    directedGraph.addEdge(5, 2);
    directedGraph.addEdge(5, 0);
    directedGraph.addEdge(4, 0);
    directedGraph.addEdge(4, 1);
    directedGraph.addEdge(2, 3);
    directedGraph.addEdge(3, 1);

    auto sorted = GraphAlgorithms<int>::topologicalSort(directedGraph);

    ASSERT_FALSE(sorted.empty());
    EXPECT_EQ(sorted.size(), 6);

    // Verify topological order
    std::unordered_map<int, size_t> position;
    for (size_t i = 0; i < sorted.size(); ++i) {
        position[sorted[i]] = i;
    }

    // All edges should go from earlier to later in the sorted order
    EXPECT_LT(position[5], position[2]);
    EXPECT_LT(position[5], position[0]);
    EXPECT_LT(position[4], position[0]);
    EXPECT_LT(position[4], position[1]);
    EXPECT_LT(position[2], position[3]);
    EXPECT_LT(position[3], position[1]);
}

TEST_F(GraphAlgorithmsTest, TopologicalSortWithCycle) {
    directedGraph.addNode(1);
    directedGraph.addNode(2);
    directedGraph.addNode(3);
    directedGraph.addEdge(1, 2);
    directedGraph.addEdge(2, 3);
    directedGraph.addEdge(3, 1); // Creates cycle

    auto sorted = GraphAlgorithms<int>::topologicalSort(directedGraph);
    EXPECT_TRUE(sorted.empty()); // Should return empty for cyclic graphs
}

// Minimum spanning tree tests
TEST_F(GraphAlgorithmsTest, MinimumSpanningTree) {
    undirectedGraph.addNode(0);
    undirectedGraph.addNode(1);
    undirectedGraph.addNode(2);
    undirectedGraph.addNode(3);

    undirectedGraph.addEdge(0, 1, 10.0);
    undirectedGraph.addEdge(0, 2, 6.0);
    undirectedGraph.addEdge(0, 3, 5.0);
    undirectedGraph.addEdge(1, 3, 15.0);
    undirectedGraph.addEdge(2, 3, 4.0);

    auto mst = GraphAlgorithms<int>::minimumSpanningTree(undirectedGraph);

    EXPECT_EQ(mst.nodeCount(), 4);
    EXPECT_EQ(mst.edgeCount(), 3); // MST should have V-1 edges
}

// All pairs shortest paths tests
TEST_F(GraphAlgorithmsTest, AllPairsShortestPaths) {
    undirectedGraph.addNode(1);
    undirectedGraph.addNode(2);
    undirectedGraph.addNode(3);

    undirectedGraph.addEdge(1, 2, 1.0);
    undirectedGraph.addEdge(2, 3, 2.0);

    auto allPaths = GraphAlgorithms<int>::allPairsShortestPaths(undirectedGraph);

    EXPECT_DOUBLE_EQ(allPaths[1][1], 0.0);
    EXPECT_DOUBLE_EQ(allPaths[1][2], 1.0);
    EXPECT_DOUBLE_EQ(allPaths[1][3], 3.0);
    EXPECT_DOUBLE_EQ(allPaths[2][3], 2.0);
}

