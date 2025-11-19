#include <cwapi3d/geometry/Point3D.h>
#include <cwapi3d/geometry/Vec3D.h>
#include <cwapi3d/geometry/Plane3D.h>
#include <cwapi3d/geometry/CoordinateFrame3D.h>
#include <cwapi3d/geometry/BoundingBox.h>
#include <iostream>
#include <iomanip>

using namespace CwAPI3D::Geometry;

void printPoint(const std::string& label, const Point3D& p) {
    std::cout << label << ": (" << std::fixed << std::setprecision(2)
              << p.x() << ", " << p.y() << ", " << p.z() << ")\n";
}

void printVec(const std::string& label, const Vec3D& v) {
    std::cout << label << ": (" << std::fixed << std::setprecision(2)
              << v.x() << ", " << v.y() << ", " << v.z() << ")\n";
}

int main() {
    std::cout << "=== CwAPI3D.Ex Geometry Examples ===\n\n";
    
    // Example 1: Basic Point and Vector Operations
    std::cout << "1. Basic Point and Vector Operations\n";
    std::cout << "--------------------------------------\n";

    const Point3D p1(1.0, 2.0, 3.0);
    const Point3D p2(4.0, 5.0, 6.0);
    
    printPoint("Point 1", p1);
    printPoint("Point 2", p2);

    const double distance = p1.distanceTo(p2);
    std::cout << "Distance: " << std::fixed << std::setprecision(2) << distance << "\n\n";

    const Vec3D v1(1.0, 0.0, 0.0);
    const Vec3D v2(0.0, 1.0, 0.0);
    
    printVec("Vector 1", v1);
    printVec("Vector 2", v2);

    const Vec3D cross = v1.cross(v2);
    printVec("Cross product", cross);

    const double dot = v1.dot(v2);
    std::cout << "Dot product: " << dot << "\n\n";
    
    // Example 2: Plane Operations
    std::cout << "2. Plane Operations\n";
    std::cout << "-------------------\n";

    const Plane3D plane(Point3D(0.0, 0.0, 5.0), Vec3D::unitZ());
    const Point3D testPoint(3.0, 4.0, 10.0);

    const double distToPlane = plane.distanceToPoint(testPoint);
    const Point3D projected = plane.projectPoint(testPoint);
    
    printPoint("Test point", testPoint);
    std::cout << "Distance to plane: " << distToPlane << "\n";
    printPoint("Projected point", projected);
    std::cout << "\n";
    
    // Example 3: Coordinate Frame Transformations
    std::cout << "3. Coordinate Frame Transformations\n";
    std::cout << "------------------------------------\n";

    const CoordinateFrame3D frame(
        Point3D(10.0, 0.0, 0.0),
        Vec3D::unitX(),
        Vec3D::unitY(),
        Vec3D::unitZ()
    );

    const Point3D worldPoint(15.0, 5.0, 3.0);
    const Point3D localPoint = frame.transformToLocal(worldPoint);
    const Point3D backToWorld = frame.transformToWorld(localPoint);
    
    printPoint("World point", worldPoint);
    printPoint("Local point", localPoint);
    printPoint("Back to world", backToWorld);
    std::cout << "\n";
    
    // Example 4: Bounding Box
    std::cout << "4. Bounding Box Operations\n";
    std::cout << "--------------------------\n";

    const AABB box(Point3D(0.0, 0.0, 0.0), Point3D(10.0, 10.0, 10.0));
    
    printPoint("Box min", box.min());
    printPoint("Box max", box.max());
    printPoint("Box center", box.center());
    std::cout << "Box volume: " << box.volume() << "\n";

    const Point3D testPoint1(5.0, 5.0, 5.0);
    const Point3D testPoint2(15.0, 5.0, 5.0);
    
    std::cout << "Point (5,5,5) in box: " << (box.contains(testPoint1) ? "yes" : "no") << "\n";
    std::cout << "Point (15,5,5) in box: " << (box.contains(testPoint2) ? "yes" : "no") << "\n";
    
    return 0;
}
