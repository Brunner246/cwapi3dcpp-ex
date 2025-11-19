#include <cwapi3d/geometry/BoundingBox.h>
#include <cwapi3d/geometry/CoordinateFrame3D.h>
#include <cwapi3d/geometry/Point3D.h>
#include <cwapi3d/geometry/Vec3D.h>

#include <iostream>
#include <iomanip>
#include <numbers>

using namespace CwAPI3D::Geometry;

void printOBB(const OBB& obb, const std::string& name) {
    std::cout << "\n" << name << ":\n";
    std::cout << "  Center: (" << obb.center().x() << ", " 
              << obb.center().y() << ", " << obb.center().z() << ")\n";
    std::cout << "  Half Extents: (" << obb.halfExtents().x() << ", " 
              << obb.halfExtents().y() << ", " << obb.halfExtents().z() << ")\n";
    std::cout << "  Size: (" << obb.size().x() << ", " 
              << obb.size().y() << ", " << obb.size().z() << ")\n";
    std::cout << "  Volume: " << obb.volume() << "\n";
    
    const auto& frame = obb.frame();
    std::cout << "  X-Axis: (" << frame.xAxis().x() << ", " 
              << frame.xAxis().y() << ", " << frame.xAxis().z() << ")\n";
    std::cout << "  Y-Axis: (" << frame.yAxis().x() << ", " 
              << frame.yAxis().y() << ", " << frame.yAxis().z() << ")\n";
    std::cout << "  Z-Axis: (" << frame.zAxis().x() << ", " 
              << frame.zAxis().y() << ", " << frame.zAxis().z() << ")\n";
}

void example_basic_obb() {
    std::cout << "\n=== Example 1: Basic OBB Creation ===\n";
    
    // Create a simple AABB
    const AABB aabb(Point3D(0.0, 0.0, 0.0), Point3D(10.0, 20.0, 30.0));
    std::cout << "AABB: min=(" << aabb.min().x() << ", " << aabb.min().y() 
              << ", " << aabb.min().z() << "), max=(" << aabb.max().x() 
              << ", " << aabb.max().y() << ", " << aabb.max().z() << ")\n";
    
    // Convert to OBB (world-aligned)
    const OBB obb = OBB::fromAABB(aabb);
    printOBB(obb, "OBB from AABB");
    
    std::cout << "\nTest containment:\n";
    std::cout << "  Contains (5, 10, 15): " 
              << (obb.contains(Point3D(5.0, 10.0, 15.0)) ? "yes" : "no") << "\n";
    std::cout << "  Contains (100, 100, 100): " 
              << (obb.contains(Point3D(100.0, 100.0, 100.0)) ? "yes" : "no") << "\n";
}

void example_rotated_obb() {
    std::cout << "\n=== Example 2: OBB with Rotated Frame ===\n";
    
    // Create a coordinate frame rotated 45 degrees around Z axis
    constexpr double angle = std::numbers::pi / 4.0; // 45 degrees
    const Vec3D xAxis(std::cos(angle), std::sin(angle), 0.0);
    const Vec3D yAxis(-std::sin(angle), std::cos(angle), 0.0);
    const Vec3D zAxis(0.0, 0.0, 1.0);
    const Point3D origin(10.0, 20.0, 0.0);

    const CoordinateFrame3D rotatedFrame(origin, xAxis, yAxis, zAxis);
    
    // Create OBB with this rotated frame
    const Vec3D halfExtents(5.0, 3.0, 2.0);
    const OBB obb(rotatedFrame, halfExtents);
    
    printOBB(obb, "Rotated OBB");
    
    // Convert back to AABB to see the bounding box in world space
    const AABB worldAABB = obb.toAABB();
    std::cout << "\nWorld-space AABB: min=(" << worldAABB.min().x() << ", " 
              << worldAABB.min().y() << ", " << worldAABB.min().z() 
              << "), max=(" << worldAABB.max().x() << ", " 
              << worldAABB.max().y() << ", " << worldAABB.max().z() << ")\n";
}

void example_element_aligned_obb() {
    std::cout << "\n=== Example 3: OBB Aligned to Element Coordinate System ===\n";
    
    // Simulate a structural element (e.g., a beam) with its own local coordinate system
    // The element is located at (10, 20, 5) and its longitudinal axis points in direction (1, 1, 0)
    const Point3D elementPosition(10.0, 20.0, 5.0);
    const Vec3D longitudinalAxis = Vec3D(1.0, 1.0, 0.0).normalized();
    
    // Create coordinate frame for the element
    const CoordinateFrame3D elementFrame = CoordinateFrame3D::fromOriginAndZ(
        elementPosition, 
        longitudinalAxis
    );
    
    std::cout << "Element coordinate system:\n";
    std::cout << "  Origin: (" << elementFrame.origin().x() << ", " 
              << elementFrame.origin().y() << ", " << elementFrame.origin().z() << ")\n";
    std::cout << "  Local Z (longitudinal): (" << elementFrame.zAxis().x() << ", " 
              << elementFrame.zAxis().y() << ", " << elementFrame.zAxis().z() << ")\n";
    
    // Define some points that represent geometry attached to this element
    const std::vector<Point3D> geometryPoints = {
        Point3D(9.5, 19.5, 4.5),
        Point3D(10.5, 20.5, 5.5),
        Point3D(8.5, 18.5, 4.0),
        Point3D(11.5, 21.5, 6.0),
        Point3D(9.0, 19.0, 5.0),
        Point3D(11.0, 21.0, 5.0)
    };
    
    // Create OBB that fits these points in the element's coordinate system
    const OBB elementOBB = OBB::fromPoints(geometryPoints, elementFrame);
    
    printOBB(elementOBB, "Element-aligned OBB");
    
    std::cout << "\nVerify all points are contained:\n";
    for (size_t i = 0; i < geometryPoints.size(); ++i) {
        const auto& pt = geometryPoints[i];
        const bool contained = elementOBB.contains(pt);
        std::cout << "  Point " << i << " (" << pt.x() << ", " << pt.y() 
                  << ", " << pt.z() << "): " << (contained ? "yes" : "no") << "\n";
    }
}

void example_obb_intersection() {
    std::cout << "\n=== Example 4: OBB Intersection Tests ===\n";
    
    // Create two OBBs
    const OBB obb1 = OBB::fromCenterAndSize(
        Point3D(0.0, 0.0, 0.0),
        Vec3D(10.0, 10.0, 10.0)
    );

    const OBB obb2 = OBB::fromCenterAndSize(
        Point3D(8.0, 0.0, 0.0),
        Vec3D(10.0, 10.0, 10.0)
    );

    const OBB obb3 = OBB::fromCenterAndSize(
        Point3D(20.0, 0.0, 0.0),
        Vec3D(10.0, 10.0, 10.0)
    );
    
    std::cout << "OBB1 center at (0, 0, 0), size (10, 10, 10)\n";
    std::cout << "OBB2 center at (8, 0, 0), size (10, 10, 10)\n";
    std::cout << "OBB3 center at (20, 0, 0), size (10, 10, 10)\n\n";
    
    std::cout << "Intersection tests:\n";
    std::cout << "  OBB1 intersects OBB2: " 
              << (obb1.intersects(obb2) ? "yes" : "no") << "\n";
    std::cout << "  OBB1 intersects OBB3: " 
              << (obb1.intersects(obb3) ? "yes" : "no") << "\n";
    std::cout << "  OBB2 intersects OBB3: " 
              << (obb2.intersects(obb3) ? "yes" : "no") << "\n";
}

void example_obb_expansion() {
    std::cout << "\n=== Example 5: OBB Expansion ===\n";
    
    // Start with a small OBB
    OBB obb = OBB::fromCenterAndSize(
        Point3D(0.0, 0.0, 0.0),
        Vec3D(2.0, 2.0, 2.0)
    );
    
    printOBB(obb, "Initial OBB");
    
    // Expand to include a point
    std::cout << "\nExpanding to include point (5, 3, 2)...\n";
    obb.expand(Point3D(5.0, 3.0, 2.0));
    
    printOBB(obb, "Expanded OBB");
    
    std::cout << "\nContains (5, 3, 2): " 
              << (obb.contains(Point3D(5.0, 3.0, 2.0)) ? "yes" : "no") << "\n";
}

int main() {
    std::cout << std::fixed << std::setprecision(6);
    
    std::cout << "==============================================\n";
    std::cout << "  Oriented Bounding Box (OBB) Examples\n";
    std::cout << "==============================================\n";
    
    example_basic_obb();
    example_rotated_obb();
    example_element_aligned_obb();
    example_obb_intersection();
    example_obb_expansion();
    
    std::cout << "\n==============================================\n";
    std::cout << "  All examples completed successfully!\n";
    std::cout << "==============================================\n";
    
    return 0;
}
