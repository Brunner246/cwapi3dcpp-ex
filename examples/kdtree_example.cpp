#include <cwapi3d/geometry/KDTree.h>
#include <cwapi3d/geometry/Point3D.h>

#include <iomanip>
#include <iostream>
#include <random>

#include "cwapi3d/geometry/BoundingBox.h"

using namespace cwapi3d::geometry;

void printPoint(const std::string& label, const Point3D& p) {
    std::cout << label << ": (" << std::fixed << std::setprecision(2) << p.x() << ", " << p.y()
              << ", " << p.z() << ")\n";
}

int main() {
    std::cout << "=== KDTree Examples ===\n\n";

    // Generate random points
    std::cout << "Generating random points...\n";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 100.0);

    std::vector<std::pair<Point3D, int>> points;
    for (int i = 0; i < 1000; ++i) {
        Point3D p(dis(gen), dis(gen), dis(gen));
        points.emplace_back(p, i);
    }
    std::cout << "Generated " << points.size() << " points\n\n";

    // Example 1: KDTree Nearest Neighbor
    std::cout << "1. KDTree Nearest Neighbor Search\n";
    std::cout << "-----------------------------------\n";

    KDTree<int> tree;
    tree.build(points);

    const Point3D queryPoint(50.0, 50.0, 50.0);
    printPoint("Query point", queryPoint);

    const auto nearest = tree.findNearest(queryPoint);
    printPoint("Nearest point", nearest.first);
    std::cout << "ID: " << nearest.second << "\n";
    std::cout << "Distance: " << queryPoint.distanceTo(nearest.first) << "\n\n";

    // Example 2: KDTree Radius Search
    std::cout << "2. KDTree Radius Search\n";
    std::cout << "-----------------------\n";

    const double radius = 10.0;
    const auto inRadius = tree.findInRadius(queryPoint, radius);

    std::cout << "Found " << inRadius.size() << " points within radius " << radius << "\n";
    if (!inRadius.empty()) {
        std::cout << "First few results:\n";
        for (size_t i = 0; i < std::min(size_t(5), inRadius.size()); ++i) {
            std::cout << "  ID " << inRadius[i].second << " at distance "
                      << queryPoint.distanceTo(inRadius[i].first) << "\n";
        }
    }
    std::cout << "\n";

    // Example 3: KDTree K-Nearest Neighbors
    std::cout << "3. KDTree K-Nearest Neighbors\n";
    std::cout << "------------------------------\n";

    const size_t k = 5;
    const auto kNearest = tree.findKNearest(queryPoint, k);

    std::cout << "Found " << kNearest.size() << " nearest neighbors:\n";
    for (size_t i = 0; i < kNearest.size(); ++i) {
        std::cout << "  #" << (i + 1) << " - ID " << kNearest[i].second << " at distance "
                  << queryPoint.distanceTo(kNearest[i].first) << "\n";
    }
    std::cout << "\n";

    std::cout << "-------------------------\n";

    return 0;
}
