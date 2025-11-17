#include <gtest/gtest.h>
#include "cwapi3d/geometry/KDTree.h"

using namespace cwapi3d::geometry;

TEST(KDTreeTest, EmptyTree) {
    const KDTree<int> tree;
    EXPECT_TRUE(tree.empty());
}

TEST(KDTreeTest, BuildFromPoints) {
    KDTree<int> tree;
    const std::vector<std::pair<Point3D, int>> points = {
        {{0.0, 0.0, 0.0}, 1},
        {{1.0, 1.0, 1.0}, 2},
        {{2.0, 2.0, 2.0}, 3},
        {{3.0, 0.0, 0.0}, 4},
        {{0.0, 3.0, 0.0}, 5}
    };
    
    tree.build(points);
    EXPECT_FALSE(tree.empty());
}

TEST(KDTreeTest, ClearTree) {
    KDTree<int> tree;
    const std::vector<std::pair<Point3D, int>> points = {
        {{0.0, 0.0, 0.0}, 1},
        {{1.0, 1.0, 1.0}, 2},
        {{2.0, 2.0, 2.0}, 3},
        {{3.0, 0.0, 0.0}, 4},
        {{0.0, 3.0, 0.0}, 5}
    };

    tree.build(points);
    tree.clear();
    EXPECT_TRUE(tree.empty());
}

TEST(KDTreeTest, FindNearestToOrigin) {
    KDTree<int> tree;
    const std::vector<std::pair<Point3D, int>> points = {
        {{0.0, 0.0, 0.0}, 1},
        {{1.0, 1.0, 1.0}, 2},
        {{2.0, 2.0, 2.0}, 3}
    };
    
    tree.build(points);
    const auto nearest = tree.findNearest(Point3D(0.1, 0.1, 0.1));
    EXPECT_EQ(nearest.second, 1);
}

TEST(KDTreeTest, FindNearestToFarPoint) {
    KDTree<int> tree;
    const std::vector<std::pair<Point3D, int>> points = {
        {{0.0, 0.0, 0.0}, 1},
        {{1.0, 1.0, 1.0}, 2},
        {{2.0, 2.0, 2.0}, 3}
    };

    tree.build(points);
    const auto nearest = tree.findNearest(Point3D(1.9, 1.9, 1.9));
    EXPECT_EQ(nearest.second, 3);
}

TEST(KDTreeTest, FindNearestToMiddlePoint) {
    KDTree<int> tree;
    const std::vector<std::pair<Point3D, int>> points = {
        {{0.0, 0.0, 0.0}, 1},
        {{1.0, 1.0, 1.0}, 2},
        {{2.0, 2.0, 2.0}, 3}
    };

    tree.build(points);
    const auto nearest = tree.findNearest(Point3D(1.0, 1.0, 1.0));
    EXPECT_EQ(nearest.second, 2);
}

TEST(KDTreeTest, FindPointsWithinRadius) {
    KDTree<int> tree;
    const std::vector<std::pair<Point3D, int>> points = {
        {{0.0, 0.0, 0.0}, 1},
        {{1.0, 0.0, 0.0}, 2},
        {{2.0, 0.0, 0.0}, 3},
        {{5.0, 0.0, 0.0}, 4}
    };
    
    tree.build(points);
    const auto results = tree.findInRadius(Point3D(0.0, 0.0, 0.0), 2.5);
    EXPECT_EQ(results.size(), 3);
}

TEST(KDTreeTest, FindPointsWithSmallRadius) {
    KDTree<int> tree;
    const std::vector<std::pair<Point3D, int>> points = {
        {{0.0, 0.0, 0.0}, 1},
        {{1.0, 0.0, 0.0}, 2},
        {{2.0, 0.0, 0.0}, 3},
        {{5.0, 0.0, 0.0}, 4}
    };

    tree.build(points);
    const auto results = tree.findInRadius(Point3D(0.0, 0.0, 0.0), 0.5);
    EXPECT_EQ(results.size(), 1);
}

TEST(KDTreeTest, FindPointsWithLargeRadius) {
    KDTree<int> tree;
    const std::vector<std::pair<Point3D, int>> points = {
        {{0.0, 0.0, 0.0}, 1},
        {{1.0, 0.0, 0.0}, 2},
        {{2.0, 0.0, 0.0}, 3},
        {{5.0, 0.0, 0.0}, 4}
    };

    tree.build(points);
    const auto results = tree.findInRadius(Point3D(0.0, 0.0, 0.0), 10.0);
    EXPECT_EQ(results.size(), 4);
}

TEST(KDTreeTest, Find3NearestNeighbors) {
    KDTree<int> tree;
    const std::vector<std::pair<Point3D, int>> points = {
        {{0.0, 0.0, 0.0}, 1},
        {{1.0, 0.0, 0.0}, 2},
        {{2.0, 0.0, 0.0}, 3},
        {{3.0, 0.0, 0.0}, 4},
        {{4.0, 0.0, 0.0}, 5}
    };
    
    tree.build(points);
    const auto results = tree.findKNearest(Point3D(2.1, 0.0, 0.0), 3);
    EXPECT_EQ(results.size(), 3);
    EXPECT_EQ(results[0].second, 3);
}

TEST(KDTreeTest, Find1NearestNeighbor) {
    KDTree<int> tree;
    const std::vector<std::pair<Point3D, int>> points = {
        {{0.0, 0.0, 0.0}, 1},
        {{1.0, 0.0, 0.0}, 2},
        {{2.0, 0.0, 0.0}, 3},
        {{3.0, 0.0, 0.0}, 4},
        {{4.0, 0.0, 0.0}, 5}
    };

    tree.build(points);
    const auto results = tree.findKNearest(Point3D(0.5, 0.0, 0.0), 1);
    EXPECT_EQ(results.size(), 1);
}

TEST(KDTreeTest, FindAllPoints) {
    KDTree<int> tree;
    const std::vector<std::pair<Point3D, int>> points = {
        {{0.0, 0.0, 0.0}, 1},
        {{1.0, 0.0, 0.0}, 2},
        {{2.0, 0.0, 0.0}, 3},
        {{3.0, 0.0, 0.0}, 4},
        {{4.0, 0.0, 0.0}, 5}
    };

    tree.build(points);
    const auto results = tree.findKNearest(Point3D(2.0, 0.0, 0.0), 5);
    EXPECT_EQ(results.size(), 5);
}

TEST(KDTreeTest, InsertIntoEmptyTree) {
    KDTree<int> tree;
    tree.insert(Point3D(1.0, 2.0, 3.0), 42);
    EXPECT_FALSE(tree.empty());

    const auto nearest = tree.findNearest(Point3D(1.0, 2.0, 3.0));
    EXPECT_EQ(nearest.second, 42);
}
