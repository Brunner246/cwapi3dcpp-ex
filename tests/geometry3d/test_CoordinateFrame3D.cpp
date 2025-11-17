#include <gtest/gtest.h>
#include "cwapi3d/geometry/CoordinateFrame3D.h"

using namespace cwapi3d::geometry;

TEST(CoordinateFrame3DTest, DefaultConstructor) {
    const CoordinateFrame3D frame;
    EXPECT_TRUE(frame.origin() == Point3D(0.0, 0.0, 0.0));
    EXPECT_TRUE(frame.xAxis() == Vec3D::unitX());
    EXPECT_TRUE(frame.yAxis() == Vec3D::unitY());
    EXPECT_TRUE(frame.zAxis() == Vec3D::unitZ());
}

TEST(CoordinateFrame3DTest, WorldFrameFactory) {
    const auto frame = CoordinateFrame3D::worldFrame();
    EXPECT_TRUE(frame.origin() == Point3D(0.0, 0.0, 0.0));
    EXPECT_TRUE(frame.xAxis() == Vec3D::unitX());
}

TEST(CoordinateFrame3DTest, WorldToLocal) {
    const CoordinateFrame3D frame(
        Point3D(10.0, 0.0, 0.0),
        Vec3D::unitX(),
        Vec3D::unitY(),
        Vec3D::unitZ()
    );

    const Point3D worldPoint(15.0, 5.0, 3.0);
    const Point3D localPoint = frame.transformToLocal(worldPoint);

    EXPECT_NEAR(localPoint.x(), 5.0, 1e-9);
    EXPECT_NEAR(localPoint.y(), 5.0, 1e-9);
    EXPECT_NEAR(localPoint.z(), 3.0, 1e-9);
}

TEST(CoordinateFrame3DTest, LocalToWorld) {
    const CoordinateFrame3D frame(
        Point3D(10.0, 0.0, 0.0),
        Vec3D::unitX(),
        Vec3D::unitY(),
        Vec3D::unitZ()
    );

    const Point3D localPoint(5.0, 5.0, 3.0);
    const Point3D worldPoint = frame.transformToWorld(localPoint);

    EXPECT_NEAR(worldPoint.x(), 15.0, 1e-9);
    EXPECT_NEAR(worldPoint.y(), 5.0, 1e-9);
    EXPECT_NEAR(worldPoint.z(), 3.0, 1e-9);
}

TEST(CoordinateFrame3DTest, RoundTripTransformation) {
    const CoordinateFrame3D frame(
        Point3D(10.0, 0.0, 0.0),
        Vec3D::unitX(),
        Vec3D::unitY(),
        Vec3D::unitZ()
    );

    const Point3D worldPoint(15.0, 5.0, 3.0);
    const Point3D localPoint = frame.transformToLocal(worldPoint);
    const Point3D backToWorld = frame.transformToWorld(localPoint);

    EXPECT_NEAR(backToWorld.x(), worldPoint.x(), 1e-9);
    EXPECT_NEAR(backToWorld.y(), worldPoint.y(), 1e-9);
    EXPECT_NEAR(backToWorld.z(), worldPoint.z(), 1e-9);
}

TEST(CoordinateFrame3DTest, WorldVectorToLocal) {
    const CoordinateFrame3D frame(
        Point3D(0.0, 0.0, 0.0),
        Vec3D::unitX(),
        Vec3D::unitY(),
        Vec3D::unitZ()
    );

    const Vec3D worldVec(1.0, 2.0, 3.0);
    const Vec3D localVec = frame.transformVectorToLocal(worldVec);

    EXPECT_NEAR(localVec.x(), 1.0, 1e-9);
    EXPECT_NEAR(localVec.y(), 2.0, 1e-9);
    EXPECT_NEAR(localVec.z(), 3.0, 1e-9);
}

TEST(CoordinateFrame3DTest, LocalVectorToWorld) {
    const CoordinateFrame3D frame(
        Point3D(0.0, 0.0, 0.0),
        Vec3D::unitX(),
        Vec3D::unitY(),
        Vec3D::unitZ()
    );

    const Vec3D localVec(1.0, 2.0, 3.0);
    const Vec3D worldVec = frame.transformVectorToWorld(localVec);

    EXPECT_NEAR(worldVec.x(), 1.0, 1e-9);
    EXPECT_NEAR(worldVec.y(), 2.0, 1e-9);
    EXPECT_NEAR(worldVec.z(), 3.0, 1e-9);
}
