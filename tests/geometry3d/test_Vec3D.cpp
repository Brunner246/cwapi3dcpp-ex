#include <gtest/gtest.h>
#include "cwapi3d/geometry/Vec3D.h"

using namespace CwAPI3D::Geometry;

TEST(Vec3DTest, DefaultConstructor) {
    const Vec3D v;
    EXPECT_DOUBLE_EQ(v.x(), 0.0);
    EXPECT_DOUBLE_EQ(v.y(), 0.0);
    EXPECT_DOUBLE_EQ(v.z(), 0.0);
}

TEST(Vec3DTest, ParameterizedConstructor) {
    const Vec3D v(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(v.x(), 1.0);
    EXPECT_DOUBLE_EQ(v.y(), 2.0);
    EXPECT_DOUBLE_EQ(v.z(), 3.0);
}

TEST(Vec3DTest, StaticUnitVectors) {
    const auto ux = Vec3D::unitX();
    const auto uy = Vec3D::unitY();
    const auto uz = Vec3D::unitZ();
    const auto zero = Vec3D::zero();

    EXPECT_DOUBLE_EQ(ux.x(), 1.0);
    EXPECT_DOUBLE_EQ(uy.y(), 1.0);
    EXPECT_DOUBLE_EQ(uz.z(), 1.0);
    EXPECT_DOUBLE_EQ(zero.length(), 0.0);
}

TEST(Vec3DTest, Length) {
    const Vec3D v(3.0, 4.0, 0.0);
    EXPECT_NEAR(v.length(), 5.0, 1e-9);
}

TEST(Vec3DTest, LengthSquared) {
    const Vec3D v(3.0, 4.0, 0.0);
    EXPECT_NEAR(v.lengthSquared(), 25.0, 1e-9);
}

TEST(Vec3DTest, NormalizedReturnsUnitVector) {
    const Vec3D v(3.0, 4.0, 0.0);
    const Vec3D normalized = v.normalized();
    EXPECT_NEAR(normalized.length(), 1.0, 1e-9);
    EXPECT_NEAR(normalized.x(), 0.6, 1e-9);
    EXPECT_NEAR(normalized.y(), 0.8, 1e-9);
}

TEST(Vec3DTest, NormalizeModifiesInPlace) {
    const Vec3D v(3.0, 4.0, 0.0);
    Vec3D v2 = v;
    v2.normalize();
    EXPECT_NEAR(v2.length(), 1.0, 1e-9);
}

TEST(Vec3DTest, DotProductPerpendicular) {
    const Vec3D v1(1.0, 0.0, 0.0);
    const Vec3D v2(0.0, 1.0, 0.0);
    EXPECT_NEAR(v1.dot(v2), 0.0, 1e-9);
}

TEST(Vec3DTest, DotProductSameVector) {
    const Vec3D v1(1.0, 0.0, 0.0);
    EXPECT_NEAR(v1.dot(v1), 1.0, 1e-9);
}

TEST(Vec3DTest, DotProductAngledVectors) {
    const Vec3D v1(1.0, 0.0, 0.0);
    const Vec3D v3(1.0, 1.0, 0.0);
    EXPECT_NEAR(v1.dot(v3), 1.0, 1e-9);
}

TEST(Vec3DTest, CrossProductXCrossYEqualsZ) {
    const Vec3D v1 = Vec3D::unitX();
    const Vec3D v2 = Vec3D::unitY();
    const Vec3D cross = v1.cross(v2);
    EXPECT_NEAR(cross.x(), 0.0, 1e-9);
    EXPECT_NEAR(cross.y(), 0.0, 1e-9);
    EXPECT_NEAR(cross.z(), 1.0, 1e-9);
}

TEST(Vec3DTest, CrossProductAntiCommutative) {
    const Vec3D v1 = Vec3D::unitX();
    const Vec3D v2 = Vec3D::unitY();
    const Vec3D cross1 = v1.cross(v2);
    const Vec3D cross2 = v2.cross(v1);
    EXPECT_TRUE(cross1 == -cross2);
}

TEST(Vec3DTest, Addition) {
    const Vec3D v1(1.0, 2.0, 3.0);
    const Vec3D v2(4.0, 5.0, 6.0);
    const Vec3D sum = v1 + v2;
    EXPECT_DOUBLE_EQ(sum.x(), 5.0);
    EXPECT_DOUBLE_EQ(sum.y(), 7.0);
    EXPECT_DOUBLE_EQ(sum.z(), 9.0);
}

TEST(Vec3DTest, Subtraction) {
    const Vec3D v1(1.0, 2.0, 3.0);
    const Vec3D v2(4.0, 5.0, 6.0);
    const Vec3D diff = v2 - v1;
    EXPECT_DOUBLE_EQ(diff.x(), 3.0);
    EXPECT_DOUBLE_EQ(diff.y(), 3.0);
    EXPECT_DOUBLE_EQ(diff.z(), 3.0);
}

TEST(Vec3DTest, ScalarMultiplication) {
    const Vec3D v1(1.0, 2.0, 3.0);
    const Vec3D scaled = v1 * 2.0;
    EXPECT_DOUBLE_EQ(scaled.x(), 2.0);
    EXPECT_DOUBLE_EQ(scaled.y(), 4.0);
    EXPECT_DOUBLE_EQ(scaled.z(), 6.0);
}

TEST(Vec3DTest, Negation) {
    const Vec3D v1(1.0, 2.0, 3.0);
    const Vec3D neg = -v1;
    EXPECT_DOUBLE_EQ(neg.x(), -1.0);
    EXPECT_DOUBLE_EQ(neg.y(), -2.0);
    EXPECT_DOUBLE_EQ(neg.z(), -3.0);
}
