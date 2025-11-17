#include <gtest/gtest.h>
#include "cwapi3d/geometry/Point3D.h"

using namespace cwapi3d::geometry;

TEST(Point3DTest, DefaultConstructor) {
    const Point3D p;
    EXPECT_DOUBLE_EQ(p.x(), 0.0);
    EXPECT_DOUBLE_EQ(p.y(), 0.0);
    EXPECT_DOUBLE_EQ(p.z(), 0.0);
}

TEST(Point3DTest, ParameterizedConstructor) {
    const Point3D p(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(p.x(), 1.0);
    EXPECT_DOUBLE_EQ(p.y(), 2.0);
    EXPECT_DOUBLE_EQ(p.z(), 3.0);
}

TEST(Point3DTest, Distance) {
    const Point3D p1(0.0, 0.0, 0.0);
    const Point3D p2(3.0, 4.0, 0.0);

    const double dist = p1.distanceTo(p2);
    EXPECT_NEAR(dist, 5.0, 1e-9);
}

TEST(Point3DTest, DistanceSquared) {
    const Point3D p1(0.0, 0.0, 0.0);
    const Point3D p2(3.0, 4.0, 0.0);

    const double distSq = p1.distanceSquaredTo(p2);
    EXPECT_NEAR(distSq, 25.0, 1e-9);
}

TEST(Point3DTest, Addition) {
    const Point3D p1(1.0, 2.0, 3.0);
    const Point3D p2(4.0, 5.0, 6.0);

    const Point3D sum = p1 + p2;
    EXPECT_DOUBLE_EQ(sum.x(), 5.0);
    EXPECT_DOUBLE_EQ(sum.y(), 7.0);
    EXPECT_DOUBLE_EQ(sum.z(), 9.0);
}

TEST(Point3DTest, Subtraction) {
    const Point3D p1(1.0, 2.0, 3.0);
    const Point3D p2(4.0, 5.0, 6.0);

    const Point3D diff = p2 - p1;
    EXPECT_DOUBLE_EQ(diff.x(), 3.0);
    EXPECT_DOUBLE_EQ(diff.y(), 3.0);
    EXPECT_DOUBLE_EQ(diff.z(), 3.0);
}

TEST(Point3DTest, ScalarMultiplication) {
    const Point3D p1(1.0, 2.0, 3.0);

    const Point3D scaled = p1 * 2.0;
    EXPECT_DOUBLE_EQ(scaled.x(), 2.0);
    EXPECT_DOUBLE_EQ(scaled.y(), 4.0);
    EXPECT_DOUBLE_EQ(scaled.z(), 6.0);
}

TEST(Point3DTest, ScalarDivision) {
    const Point3D p2(4.0, 5.0, 6.0);

    const Point3D divided = p2 / 2.0;
    EXPECT_DOUBLE_EQ(divided.x(), 2.0);
    EXPECT_DOUBLE_EQ(divided.y(), 2.5);
    EXPECT_DOUBLE_EQ(divided.z(), 3.0);
}

TEST(Point3DTest, CompoundAssignment) {
    const Point3D p1(1.0, 2.0, 3.0);
    const Point3D p2(4.0, 5.0, 6.0);

    Point3D p = p1;
    p += p2;
    EXPECT_DOUBLE_EQ(p.x(), 5.0);
    EXPECT_DOUBLE_EQ(p.y(), 7.0);
    EXPECT_DOUBLE_EQ(p.z(), 9.0);
}

TEST(Point3DTest, Equality) {
    const Point3D p1(1.0, 2.0, 3.0);
    const Point3D p2(1.0, 2.0, 3.0);
    const Point3D p3(1.0, 2.0, 3.1);
    
    EXPECT_TRUE(p1 == p2);
    EXPECT_FALSE(p1 == p3);
}

TEST(Point3DTest, Inequality) {
    const Point3D p1(1.0, 2.0, 3.0);
    const Point3D p3(1.0, 2.0, 3.1);

    EXPECT_TRUE(p1 != p3);
}
