#include <iostream>
#include <memory>

// Include CwAPI3D.Ex headers
#include "cwapi3d/geometry/Point3D.h"
#include "cwapi3d/geometry/Vec3D.h"
#include "cwapi3d/geometry/CoordinateFrame3D.h"
#include "cwapi3d/geometry/KDTree.h"
#include "cwapi3d/composite/Component.h"

void demonstrateGeometry() {
    std::cout << "=== Geometry3D Examples ===\n\n";

    using namespace cwapi3d::geometry;

    // 1. Basic point operations
    std::cout << "1. Point Distance Calculation:\n";
    Point3D p1(0.0, 0.0, 0.0);
    Point3D p2(3.0, 4.0, 0.0);
    double distance = p1.distanceTo(p2);
    std::cout << "   Distance from (0,0,0) to (3,4,0): " << distance << "\n\n";

    // 2. Vector operations
    std::cout << "2. Vector Operations:\n";
    Vec3D v1(1.0, 0.0, 0.0);
    Vec3D v2(0.0, 1.0, 0.0);
    Vec3D cross = v1.cross(v2);
    std::cout << "   X cross Y = (" << cross.x() << ", " << cross.y() << ", " << cross.z() << ")\n";

    Vec3D v3(3.0, 4.0, 0.0);
    Vec3D normalized = v3.normalized();
    std::cout << "   Normalized (3,4,0) = (" << normalized.x() << ", " << normalized.y() << ", " << normalized.z() << ")\n\n";

    // 3. Coordinate frame transformations
    std::cout << "3. Coordinate Frame Transformations:\n";
    CoordinateFrame3D frame(
        Point3D(10.0, 0.0, 0.0),  // Origin offset by 10 in X
        Vec3D::unitX(),
        Vec3D::unitY(),
        Vec3D::unitZ()
    );

    Point3D worldPoint(15.0, 5.0, 3.0);
    Point3D localPoint = frame.transformToLocal(worldPoint);
    std::cout << "   World point (15,5,3) in local coords: ("
              << localPoint.x() << ", " << localPoint.y() << ", " << localPoint.z() << ")\n\n";

    // 4. KD-Tree nearest neighbor search
    std::cout << "4. KD-Tree Nearest Neighbor Search:\n";
    KDTree<std::string> tree;
    std::vector<std::pair<Point3D, std::string>> points = {
        {{0.0, 0.0, 0.0}, "Origin"},
        {{1.0, 0.0, 0.0}, "Point A"},
        {{2.0, 0.0, 0.0}, "Point B"},
        {{5.0, 0.0, 0.0}, "Point C"}
    };
    tree.build(points);

    Point3D queryPoint(1.5, 0.0, 0.0);
    auto nearest = tree.findNearest(queryPoint);
    std::cout << "   Nearest to (1.5,0,0): " << nearest.second << "\n";

    auto inRadius = tree.findInRadius(Point3D(0.0, 0.0, 0.0), 2.5);
    std::cout << "   Points within radius 2.5 of origin: " << inRadius.size() << "\n\n";
}

void demonstrateComposite() {
    std::cout << "=== Composite Pattern Examples ===\n\n";

    using namespace cwapi3d::composite;

    // Create a building hierarchy
    std::cout << "Creating building hierarchy...\n";
    auto building = std::make_unique<Composite>("Office Building");

    // Floor 1
    auto floor1 = std::make_unique<Composite>("Floor 1");
    floor1->addChild(std::make_unique<Leaf>("Room 101 - Reception"));
    floor1->addChild(std::make_unique<Leaf>("Room 102 - Conference"));
    floor1->addChild(std::make_unique<Leaf>("Room 103 - Office"));

    // Floor 2
    auto floor2 = std::make_unique<Composite>("Floor 2");
    floor2->addChild(std::make_unique<Leaf>("Room 201 - CEO Office"));
    floor2->addChild(std::make_unique<Leaf>("Room 202 - Meeting Room"));

    building->addChild(std::move(floor1));
    building->addChild(std::move(floor2));

    std::cout << "Building structure created with " << building->childCount() << " floors\n\n";

    // Traverse and print hierarchy
    std::cout << "Building hierarchy:\n";
    int depth = 0;
    building->traverse([&depth](const Component& comp) {
        // Calculate depth based on parent chain
        int currentDepth = 0;
        const Component* current = &comp;
        while (current->parent() != nullptr) {
            currentDepth++;
            current = current->parent();
        }

        std::string indent(currentDepth * 2, ' ');
        std::string type = comp.isComposite() ? "[Composite]" : "[Leaf]";
        std::cout << indent << type << " " << comp.name() << "\n";
    });
    std::cout << "\n";

    // Find a specific child
    auto found = building->findChild("Floor 1");
    if (found) {
        std::cout << "Found component: " << found->name();
        if (found->isComposite()) {
            auto composite = static_cast<const Composite*>(found);
            std::cout << " with " << composite->childCount() << " children\n";
        }
        std::cout << "\n";
    }
}

int main() {
    std::cout << "======================================\n";
    std::cout << "  CwAPI3D.Ex FetchContent Example\n";
    std::cout << "======================================\n\n";

    try {
        demonstrateGeometry();
        demonstrateComposite();

        std::cout << "======================================\n";
        std::cout << "  All examples completed successfully!\n";
        std::cout << "======================================\n";

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

