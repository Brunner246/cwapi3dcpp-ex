#pragma once

#include "Point3D.h"
#include <vector>
#include <memory>
#include <functional>
#include <limits>

namespace cwapi3d::geometry {

/**
 * @brief KD-Tree for efficient 3D spatial queries
 * @tparam T The type of data associated with each point
 */
template<typename T>
class KDTree {
public:
    struct Node {
        Point3D point;
        T data;
        int axis;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        Node(const Point3D& p, const T& d, const int a)
            : point(p), data(d), axis(a), left(nullptr), right(nullptr) {}
    };

    KDTree() : mRoot(nullptr) {}

    // Build tree from points
    void build(const std::vector<std::pair<Point3D, T>>& points);

    // Insert a point
    void insert(const Point3D& point, const T& data);

    // Nearest neighbor search
    [[nodiscard]] std::pair<Point3D, T> findNearest(const Point3D& query) const;

    // Range search
    [[nodiscard]] std::vector<std::pair<Point3D, T>> findInRadius(
        const Point3D& query, double radius) const;

    // K-nearest neighbors
    [[nodiscard]] std::vector<std::pair<Point3D, T>> findKNearest(
        const Point3D& query, size_t k) const;

    // Clear the tree
    void clear() { mRoot.reset(); }

    [[nodiscard]] bool empty() const noexcept { return mRoot == nullptr; }

private:
    std::unique_ptr<Node> mRoot;

    std::unique_ptr<Node> buildRecursive(
        std::vector<std::pair<Point3D, T>>& points,
        size_t start, size_t end, int depth);

    void insertRecursive(std::unique_ptr<Node>& node, const Point3D& point, const T& data, int depth);

    void findNearestRecursive(
        const Node* node,
        const Point3D& query,
        const Node*& bestNode,
        double& bestDistSq) const;

    void findInRadiusRecursive(
        const Node* node,
        const Point3D& query,
        double radiusSq,
        std::vector<std::pair<Point3D, T>>& results) const;
};

} // namespace cwapi3d::geometry

// Template implementation
#include "KDTree.inl"
