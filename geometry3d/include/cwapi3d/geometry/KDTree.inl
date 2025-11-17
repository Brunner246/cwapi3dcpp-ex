#pragma once

#include <algorithm>
#include <cmath>
#include <queue>
#include <stdexcept>

namespace cwapi3d::geometry {

template<typename T>
void KDTree<T>::build(const std::vector<std::pair<Point3D, T>>& points) {
    if (points.empty()) {
        mRoot.reset();
        return;
    }
    
    std::vector<std::pair<Point3D, T>> pointsCopy = points;
    mRoot = buildRecursive(pointsCopy, 0, pointsCopy.size(), 0);
}

template<typename T>
void KDTree<T>::insert(const Point3D& point, const T& data) {
    insertRecursive(mRoot, point, data, 0);
}

template<typename T>
std::pair<Point3D, T> KDTree<T>::findNearest(const Point3D& query) const {
    if (!mRoot) {
        throw std::runtime_error("KDTree is empty");
    }
    
    const Node* bestNode = nullptr;
    double bestDistSq = std::numeric_limits<double>::max();
    
    findNearestRecursive(mRoot.get(), query, bestNode, bestDistSq);
    
    return {bestNode->point, bestNode->data};
}

template<typename T>
std::vector<std::pair<Point3D, T>> KDTree<T>::findInRadius(
    const Point3D& query,
                                                           const double radius) const {
    
    std::vector<std::pair<Point3D, T>> results;
    const double radiusSq = radius * radius;
    
    if (mRoot) {
        findInRadiusRecursive(mRoot.get(), query, radiusSq, results);
    }
    
    return results;
}

template<typename T>
std::vector<std::pair<Point3D, T>> KDTree<T>::findKNearest(
    const Point3D& query, size_t k) const {
    
    if (!mRoot || k == 0) {
        return {};
    }
    
    // Use a max heap to keep track of k nearest neighbors
    auto comparator = [](const auto& a, const auto& b) {
        return a.first < b.first; // Max heap based on distance
    };
    std::priority_queue<std::pair<double, const Node*>,
                       std::vector<std::pair<double, const Node*>>,
                       decltype(comparator)> heap(comparator);
    
    std::function<void(const Node*)> search = [&](const Node* node) {
        if (!node) return;
        
        const double distSq = query.distanceSquaredTo(node->point);
        
        if (heap.size() < k) {
            heap.push({distSq, node});
        } else if (distSq < heap.top().first) {
            heap.pop();
            heap.push({distSq, node});
        }
        
        const int axis = node->axis;
        const double diff = query[axis] - node->point[axis];
        const double diffSq = diff * diff;
        
        const Node* first = diff < 0 ? node->left.get() : node->right.get();
        const Node* second = diff < 0 ? node->right.get() : node->left.get();
        
        search(first);
        
        if (heap.size() < k || diffSq < heap.top().first) {
            search(second);
        }
    };
    
    search(mRoot.get());
    
    std::vector<std::pair<Point3D, T>> results;
    results.reserve(heap.size());
    
    while (!heap.empty()) {
        results.push_back({heap.top().second->point, heap.top().second->data});
        heap.pop();
    }
    
    std::reverse(results.begin(), results.end());
    return results;
}

template<typename T>
std::unique_ptr<typename KDTree<T>::Node> KDTree<T>::buildRecursive(
    std::vector<std::pair<Point3D, T>>& points,
    size_t start, size_t end, const int depth) {
    
    if (start >= end) {
        return nullptr;
    }
    
    const int axis = depth % 3;
    const size_t mid = start + (end - start) / 2;
    
    // Partition around median
    std::nth_element(points.begin() + start,
                     points.begin() + mid,
                     points.begin() + end,
                     [axis](const auto& a, const auto& b) {
                         return a.first[axis] < b.first[axis];
                     });
    
    auto node = std::make_unique<Node>(points[mid].first, points[mid].second, axis);
    node->left = buildRecursive(points, start, mid, depth + 1);
    node->right = buildRecursive(points, mid + 1, end, depth + 1);
    
    return node;
}

template<typename T>
void KDTree<T>::insertRecursive(std::unique_ptr<Node>& node,
                                const Point3D& point, const T& data,
                                const int depth) {
    if (!node) {
        const int axis = depth % 3;
        node = std::make_unique<Node>(point, data, axis);
        return;
    }
    
    if (point[node->axis] < node->point[node->axis]) {
        insertRecursive(node->left, point, data, depth + 1);
    } else {
        insertRecursive(node->right, point, data, depth + 1);
    }
}

template<typename T>
void KDTree<T>::findNearestRecursive(
    const Node* node,
    const Point3D& query,
    const Node*& bestNode,
    double& bestDistSq) const {
    
    if (!node) return;
    
    const double distSq = query.distanceSquaredTo(node->point);
    
    if (distSq < bestDistSq) {
        bestDistSq = distSq;
        bestNode = node;
    }
    
    const int axis = node->axis;
    const double diff = query[axis] - node->point[axis];
    const double diffSq = diff * diff;
    
    const Node* first = diff < 0 ? node->left.get() : node->right.get();
    const Node* second = diff < 0 ? node->right.get() : node->left.get();
    
    findNearestRecursive(first, query, bestNode, bestDistSq);
    
    if (diffSq < bestDistSq) {
        findNearestRecursive(second, query, bestNode, bestDistSq);
    }
}

template<typename T>
void KDTree<T>::findInRadiusRecursive(
    const Node* node,
    const Point3D& query, const double radiusSq,
    std::vector<std::pair<Point3D, T>>& results) const {
    
    if (!node) return;
    
    const double distSq = query.distanceSquaredTo(node->point);
    
    if (distSq <= radiusSq) {
        results.push_back({node->point, node->data});
    }
    
    const int axis = node->axis;
    const double diff = query[axis] - node->point[axis];
    const double diffSq = diff * diff;
    
    if (diff < 0) {
        findInRadiusRecursive(node->left.get(), query, radiusSq, results);
        if (diffSq <= radiusSq) {
            findInRadiusRecursive(node->right.get(), query, radiusSq, results);
        }
    } else {
        findInRadiusRecursive(node->right.get(), query, radiusSq, results);
        if (diffSq <= radiusSq) {
            findInRadiusRecursive(node->left.get(), query, radiusSq, results);
        }
    }
}

} // namespace cwapi3d::geometry
