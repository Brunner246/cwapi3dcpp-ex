#include <gtest/gtest.h>
#include "cwapi3d/geometry/BoundingBox.h"
#include "cwapi3d/geometry/CoordinateFrame3D.h"

using namespace cwapi3d::geometry;

// ===== AABB Tests =====

TEST(AABBTest, DefaultConstructor) {
    const AABB aabb;
    EXPECT_FALSE(aabb.isValid());
}

TEST(AABBTest, ParameterizedConstructor) {
    const Point3D min(0.0, 0.0, 0.0);
    const Point3D max(10.0, 10.0, 10.0);
    const AABB aabb(min, max);
    
    EXPECT_TRUE(aabb.isValid());
    EXPECT_DOUBLE_EQ(aabb.min().x(), 0.0);
    EXPECT_DOUBLE_EQ(aabb.max().x(), 10.0);
}

TEST(AABBTest, Center) {
    const AABB aabb(Point3D(0.0, 0.0, 0.0), Point3D(10.0, 10.0, 10.0));
    const Point3D center = aabb.center();
    
    EXPECT_DOUBLE_EQ(center.x(), 5.0);
    EXPECT_DOUBLE_EQ(center.y(), 5.0);
    EXPECT_DOUBLE_EQ(center.z(), 5.0);
}

TEST(AABBTest, Size) {
    const AABB aabb(Point3D(0.0, 0.0, 0.0), Point3D(10.0, 20.0, 30.0));
    const Vec3D size = aabb.size();
    
    EXPECT_DOUBLE_EQ(size.x(), 10.0);
    EXPECT_DOUBLE_EQ(size.y(), 20.0);
    EXPECT_DOUBLE_EQ(size.z(), 30.0);
}

TEST(AABBTest, Volume) {
    const AABB aabb(Point3D(0.0, 0.0, 0.0), Point3D(2.0, 3.0, 4.0));
    EXPECT_DOUBLE_EQ(aabb.volume(), 24.0);
}

TEST(AABBTest, Contains) {
    const AABB aabb(Point3D(0.0, 0.0, 0.0), Point3D(10.0, 10.0, 10.0));
    
    EXPECT_TRUE(aabb.contains(Point3D(5.0, 5.0, 5.0)));
    EXPECT_TRUE(aabb.contains(Point3D(0.0, 0.0, 0.0)));  // On boundary
    EXPECT_TRUE(aabb.contains(Point3D(10.0, 10.0, 10.0))); // On boundary
    EXPECT_FALSE(aabb.contains(Point3D(-1.0, 5.0, 5.0)));
    EXPECT_FALSE(aabb.contains(Point3D(11.0, 5.0, 5.0)));
}

TEST(AABBTest, Intersects) {
    const AABB aabb1(Point3D(0.0, 0.0, 0.0), Point3D(10.0, 10.0, 10.0));
    const AABB aabb2(Point3D(5.0, 5.0, 5.0), Point3D(15.0, 15.0, 15.0));
    const AABB aabb3(Point3D(20.0, 20.0, 20.0), Point3D(30.0, 30.0, 30.0));
    
    EXPECT_TRUE(aabb1.intersects(aabb2));
    EXPECT_TRUE(aabb2.intersects(aabb1));
    EXPECT_FALSE(aabb1.intersects(aabb3));
}

TEST(AABBTest, Expand) {
    AABB aabb(Point3D(0.0, 0.0, 0.0), Point3D(10.0, 10.0, 10.0));
    aabb.expand(Point3D(15.0, 5.0, 5.0));
    
    EXPECT_DOUBLE_EQ(aabb.max().x(), 15.0);
    EXPECT_DOUBLE_EQ(aabb.max().y(), 10.0);
}

TEST(AABBTest, Merged) {
    const AABB aabb1(Point3D(0.0, 0.0, 0.0), Point3D(10.0, 10.0, 10.0));
    const AABB aabb2(Point3D(5.0, 5.0, 5.0), Point3D(15.0, 15.0, 15.0));
    const AABB merged = aabb1.merged(aabb2);
    
    EXPECT_DOUBLE_EQ(merged.min().x(), 0.0);
    EXPECT_DOUBLE_EQ(merged.max().x(), 15.0);
}

// ===== OBB Tests =====

TEST(OBBTest, DefaultConstructor) {
    const OBB obb;
    EXPECT_TRUE(obb.isValid());
    EXPECT_DOUBLE_EQ(obb.center().x(), 0.0);
    EXPECT_DOUBLE_EQ(obb.center().y(), 0.0);
    EXPECT_DOUBLE_EQ(obb.center().z(), 0.0);
}

TEST(OBBTest, ConstructorWithFrameAndExtents) {
    const Point3D center(5.0, 5.0, 5.0);
    CoordinateFrame3D frame;
    frame.setOrigin(center);
    const Vec3D halfExtents(2.0, 3.0, 4.0);
    
    const OBB obb(frame, halfExtents);
    
    EXPECT_DOUBLE_EQ(obb.center().x(), 5.0);
    EXPECT_DOUBLE_EQ(obb.center().y(), 5.0);
    EXPECT_DOUBLE_EQ(obb.center().z(), 5.0);
    EXPECT_DOUBLE_EQ(obb.halfExtents().x(), 2.0);
    EXPECT_DOUBLE_EQ(obb.halfExtents().y(), 3.0);
    EXPECT_DOUBLE_EQ(obb.halfExtents().z(), 4.0);
}

TEST(OBBTest, FromAABB) {
    const AABB aabb(Point3D(0.0, 0.0, 0.0), Point3D(10.0, 10.0, 10.0));
    const OBB obb = OBB::fromAABB(aabb);
    
    EXPECT_DOUBLE_EQ(obb.center().x(), 5.0);
    EXPECT_DOUBLE_EQ(obb.center().y(), 5.0);
    EXPECT_DOUBLE_EQ(obb.center().z(), 5.0);
    EXPECT_DOUBLE_EQ(obb.halfExtents().x(), 5.0);
    EXPECT_DOUBLE_EQ(obb.halfExtents().y(), 5.0);
    EXPECT_DOUBLE_EQ(obb.halfExtents().z(), 5.0);
}

TEST(OBBTest, FromAABBWithCustomFrame) {
    const AABB aabb(Point3D(0.0, 0.0, 0.0), Point3D(10.0, 10.0, 10.0));
    
    // Create a rotated frame (45 degrees around Z axis)
    const double angle = M_PI / 4.0;
    const Vec3D xAxis(std::cos(angle), std::sin(angle), 0.0);
    const Vec3D yAxis(-std::sin(angle), std::cos(angle), 0.0);
    const Vec3D zAxis(0.0, 0.0, 1.0);
    const CoordinateFrame3D frame(Point3D(0.0, 0.0, 0.0), xAxis, yAxis, zAxis);
    
    const OBB obb = OBB::fromAABB(aabb, frame);
    
    // OBB should be at AABB center but with the custom frame orientation
    EXPECT_NEAR(obb.center().x(), 5.0, 1e-9);
    EXPECT_NEAR(obb.center().y(), 5.0, 1e-9);
    EXPECT_NEAR(obb.center().z(), 5.0, 1e-9);
}

TEST(OBBTest, FromPoints) {
    std::vector<Point3D> points = {
        Point3D(0.0, 0.0, 0.0),
        Point3D(10.0, 0.0, 0.0),
        Point3D(0.0, 10.0, 0.0),
        Point3D(10.0, 10.0, 0.0)
    };
    
    const OBB obb = OBB::fromPoints(points);
    
    EXPECT_DOUBLE_EQ(obb.center().x(), 5.0);
    EXPECT_DOUBLE_EQ(obb.center().y(), 5.0);
    EXPECT_DOUBLE_EQ(obb.center().z(), 0.0);
}

TEST(OBBTest, FromCenterAndSize) {
    const Point3D center(5.0, 5.0, 5.0);
    const Vec3D size(10.0, 20.0, 30.0);
    
    const OBB obb = OBB::fromCenterAndSize(center, size);
    
    EXPECT_DOUBLE_EQ(obb.center().x(), 5.0);
    EXPECT_DOUBLE_EQ(obb.center().y(), 5.0);
    EXPECT_DOUBLE_EQ(obb.center().z(), 5.0);
    EXPECT_DOUBLE_EQ(obb.halfExtents().x(), 5.0);
    EXPECT_DOUBLE_EQ(obb.halfExtents().y(), 10.0);
    EXPECT_DOUBLE_EQ(obb.halfExtents().z(), 15.0);
}

TEST(OBBTest, Size) {
    const Vec3D halfExtents(2.0, 3.0, 4.0);
    const OBB obb(CoordinateFrame3D::worldFrame(), halfExtents);
    const Vec3D size = obb.size();
    
    EXPECT_DOUBLE_EQ(size.x(), 4.0);
    EXPECT_DOUBLE_EQ(size.y(), 6.0);
    EXPECT_DOUBLE_EQ(size.z(), 8.0);
}

TEST(OBBTest, Volume) {
    const Vec3D halfExtents(2.0, 3.0, 4.0);
    const OBB obb(CoordinateFrame3D::worldFrame(), halfExtents);
    
    EXPECT_DOUBLE_EQ(obb.volume(), 4.0 * 6.0 * 8.0);
}

TEST(OBBTest, GetCorners) {
    const Vec3D halfExtents(1.0, 1.0, 1.0);
    const OBB obb(CoordinateFrame3D::worldFrame(), halfExtents);
    const auto corners = obb.getCorners();
    
    EXPECT_EQ(corners.size(), 8);
    
    // Check that all corners are at distance sqrt(3) from center
    for (const auto& corner : corners) {
        const Vec3D diff(corner.x(), corner.y(), corner.z());
        EXPECT_NEAR(diff.length(), std::sqrt(3.0), 1e-9);
    }
}

TEST(OBBTest, ContainsPoint) {
    const Vec3D halfExtents(5.0, 5.0, 5.0);
    const OBB obb(CoordinateFrame3D::worldFrame(), halfExtents);
    
    EXPECT_TRUE(obb.contains(Point3D(0.0, 0.0, 0.0)));    // Center
    EXPECT_TRUE(obb.contains(Point3D(4.0, 4.0, 4.0)));    // Inside
    EXPECT_TRUE(obb.contains(Point3D(5.0, 0.0, 0.0)));    // On boundary
    EXPECT_FALSE(obb.contains(Point3D(6.0, 0.0, 0.0)));   // Outside
    EXPECT_FALSE(obb.contains(Point3D(10.0, 10.0, 10.0))); // Far outside
}

TEST(OBBTest, ContainsPointRotated) {
    // Create OBB rotated 45 degrees around Z axis
    const double angle = M_PI / 4.0;
    const Vec3D xAxis(std::cos(angle), std::sin(angle), 0.0);
    const Vec3D yAxis(-std::sin(angle), std::cos(angle), 0.0);
    const Vec3D zAxis(0.0, 0.0, 1.0);
    const CoordinateFrame3D frame(Point3D(0.0, 0.0, 0.0), xAxis, yAxis, zAxis);
    const Vec3D halfExtents(1.0, 1.0, 1.0);
    
    const OBB obb(frame, halfExtents);
    
    // Point at (0,0,0) should be at center and contained
    EXPECT_TRUE(obb.contains(Point3D(0.0, 0.0, 0.0)));
}

TEST(OBBTest, IntersectsOBB) {
    const Vec3D halfExtents(5.0, 5.0, 5.0);
    const OBB obb1(CoordinateFrame3D::worldFrame(), halfExtents);
    
    CoordinateFrame3D frame2;
    frame2.setOrigin(Point3D(8.0, 0.0, 0.0));
    const OBB obb2(frame2, halfExtents);
    
    CoordinateFrame3D frame3;
    frame3.setOrigin(Point3D(20.0, 0.0, 0.0));
    const OBB obb3(frame3, halfExtents);
    
    EXPECT_TRUE(obb1.intersects(obb2));  // Overlapping
    EXPECT_FALSE(obb1.intersects(obb3)); // Separated
}

TEST(OBBTest, IntersectsAABB) {
    const Vec3D halfExtents(5.0, 5.0, 5.0);
    const OBB obb(CoordinateFrame3D::worldFrame(), halfExtents);
    
    const AABB aabb1(Point3D(-2.0, -2.0, -2.0), Point3D(2.0, 2.0, 2.0));
    const AABB aabb2(Point3D(20.0, 20.0, 20.0), Point3D(30.0, 30.0, 30.0));
    
    EXPECT_TRUE(obb.intersects(aabb1));  // Overlapping
    EXPECT_FALSE(obb.intersects(aabb2)); // Separated
}

TEST(OBBTest, ExpandWithPoint) {
    Vec3D halfExtents(1.0, 1.0, 1.0);
    OBB obb(CoordinateFrame3D::worldFrame(), halfExtents);
    
    obb.expand(Point3D(5.0, 0.0, 0.0));
    
    EXPECT_DOUBLE_EQ(obb.halfExtents().x(), 5.0);
    EXPECT_DOUBLE_EQ(obb.halfExtents().y(), 1.0);
    EXPECT_DOUBLE_EQ(obb.halfExtents().z(), 1.0);
}

TEST(OBBTest, ExpandWithOBB) {
    Vec3D halfExtents1(1.0, 1.0, 1.0);
    OBB obb1(CoordinateFrame3D::worldFrame(), halfExtents1);
    
    CoordinateFrame3D frame2;
    frame2.setOrigin(Point3D(5.0, 0.0, 0.0));
    Vec3D halfExtents2(1.0, 1.0, 1.0);
    const OBB obb2(frame2, halfExtents2);
    
    obb1.expand(obb2);
    
    // The OBB should now contain all corners of obb2
    const auto corners = obb2.getCorners();
    for (const auto& corner : corners) {
        EXPECT_TRUE(obb1.contains(corner));
    }
}

TEST(OBBTest, MergedOBB) {
    Vec3D halfExtents1(1.0, 1.0, 1.0);
    const OBB obb1(CoordinateFrame3D::worldFrame(), halfExtents1);
    
    CoordinateFrame3D frame2;
    frame2.setOrigin(Point3D(5.0, 0.0, 0.0));
    Vec3D halfExtents2(1.0, 1.0, 1.0);
    const OBB obb2(frame2, halfExtents2);
    
    const OBB merged = obb1.merged(obb2);
    
    // Merged OBB should contain both original OBBs
    EXPECT_TRUE(merged.contains(obb1.center()));
    EXPECT_TRUE(merged.contains(obb2.center()));
}

TEST(OBBTest, ToAABB) {
    const Vec3D halfExtents(5.0, 5.0, 5.0);
    const OBB obb(CoordinateFrame3D::worldFrame(), halfExtents);
    const AABB aabb = obb.toAABB();
    
    EXPECT_DOUBLE_EQ(aabb.min().x(), -5.0);
    EXPECT_DOUBLE_EQ(aabb.max().x(), 5.0);
    EXPECT_DOUBLE_EQ(aabb.min().y(), -5.0);
    EXPECT_DOUBLE_EQ(aabb.max().y(), 5.0);
    EXPECT_DOUBLE_EQ(aabb.min().z(), -5.0);
    EXPECT_DOUBLE_EQ(aabb.max().z(), 5.0);
}

TEST(OBBTest, ToAABBRotated) {
    // Create OBB rotated 45 degrees around Z axis
    const double angle = M_PI / 4.0;
    const Vec3D xAxis(std::cos(angle), std::sin(angle), 0.0);
    const Vec3D yAxis(-std::sin(angle), std::cos(angle), 0.0);
    const Vec3D zAxis(0.0, 0.0, 1.0);
    const CoordinateFrame3D frame(Point3D(0.0, 0.0, 0.0), xAxis, yAxis, zAxis);
    const Vec3D halfExtents(1.0, 1.0, 1.0);
    
    const OBB obb(frame, halfExtents);
    const AABB aabb = obb.toAABB();
    
    // AABB should be larger due to rotation
    const double expectedHalfSize = std::sqrt(2.0);
    EXPECT_NEAR(aabb.min().x(), -expectedHalfSize, 1e-9);
    EXPECT_NEAR(aabb.max().x(), expectedHalfSize, 1e-9);
    EXPECT_NEAR(aabb.min().y(), -expectedHalfSize, 1e-9);
    EXPECT_NEAR(aabb.max().y(), expectedHalfSize, 1e-9);
}

TEST(OBBTest, IsValid) {
    const Vec3D validExtents(1.0, 2.0, 3.0);
    const OBB validOBB(CoordinateFrame3D::worldFrame(), validExtents);
    EXPECT_TRUE(validOBB.isValid());
    
    const Vec3D invalidExtents(-1.0, 2.0, 3.0);
    const OBB invalidOBB(CoordinateFrame3D::worldFrame(), invalidExtents);
    EXPECT_FALSE(invalidOBB.isValid());
}

TEST(OBBTest, OrientedToElementFrame) {
    // Simulate an element with its own coordinate system
    // Element is at position (10, 20, 30) with Z axis pointing in direction (1, 1, 0) normalized
    const Point3D elementOrigin(10.0, 20.0, 30.0);
    const Vec3D elementZAxis = Vec3D(1.0, 1.0, 0.0).normalized();
    const CoordinateFrame3D elementFrame = CoordinateFrame3D::fromOriginAndZ(elementOrigin, elementZAxis);
    
    // Create points in world space
    std::vector<Point3D> worldPoints = {
        Point3D(8.0, 18.0, 29.0),
        Point3D(12.0, 22.0, 31.0),
        Point3D(9.0, 19.0, 29.5),
        Point3D(11.0, 21.0, 30.5)
    };
    
    // Create OBB aligned to element coordinate system
    const OBB obb = OBB::fromPoints(worldPoints, elementFrame);
    
    // Verify the OBB uses the element's frame
    EXPECT_NEAR(obb.frame().zAxis().x(), elementZAxis.x(), 1e-9);
    EXPECT_NEAR(obb.frame().zAxis().y(), elementZAxis.y(), 1e-9);
    EXPECT_NEAR(obb.frame().zAxis().z(), elementZAxis.z(), 1e-9);
    
    // All points should be contained in the OBB
    for (const auto& point : worldPoints) {
        EXPECT_TRUE(obb.contains(point));
    }
}
