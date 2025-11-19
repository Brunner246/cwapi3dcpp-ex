#include "cwapi3d/geometry/CoordinateFrame3D.h"
#include <stdexcept>

namespace CwAPI3D::Geometry {

CoordinateFrame3D::CoordinateFrame3D() noexcept
    : mOrigin(0.0, 0.0, 0.0)
    , mX(1.0, 0.0, 0.0)
    , mY(0.0, 1.0, 0.0)
    , mZ(0.0, 0.0, 1.0) {
}

CoordinateFrame3D::CoordinateFrame3D(const Point3D& origin, const Vec3D& xAxis,
                                      const Vec3D& yAxis, const Vec3D& zAxis)
    : mOrigin(origin)
    , mX(xAxis)
    , mY(yAxis)
    , mZ(zAxis) {
    orthonormalize();
}

void CoordinateFrame3D::setAxes(const Vec3D& xAxis, const Vec3D& yAxis, const Vec3D& zAxis) {
    mX = xAxis;
    mY = yAxis;
    mZ = zAxis;
    orthonormalize();
}

void CoordinateFrame3D::orthonormalize() {
    // Normalize Z axis first
    mZ.normalize();
    
    // Make Y perpendicular to Z using Gram-Schmidt
    const double dotYZ = mY.dot(mZ);
    mY = mY - mZ * dotYZ;
    mY.normalize();

    mX = mY.cross(mZ);
    mX.normalize();
}

Point3D CoordinateFrame3D::transformToLocal(const Point3D& worldPoint) const noexcept {
    const Vec3D v(worldPoint.x() - mOrigin.x(),
                  worldPoint.y() - mOrigin.y(),
                  worldPoint.z() - mOrigin.z());
    
    return {v.dot(mX), v.dot(mY), v.dot(mZ)};
}

Point3D CoordinateFrame3D::transformToWorld(const Point3D& localPoint) const noexcept {
    return {
        mOrigin.x() + localPoint.x() * mX.x() + localPoint.y() * mY.x() + localPoint.z() * mZ.x(),
        mOrigin.y() + localPoint.x() * mX.y() + localPoint.y() * mY.y() + localPoint.z() * mZ.y(),
        mOrigin.z() + localPoint.x() * mX.z() + localPoint.y() * mY.z() + localPoint.z() * mZ.z()
    };
}

Vec3D CoordinateFrame3D::transformVectorToLocal(const Vec3D& worldVector) const noexcept {
    return {worldVector.dot(mX), worldVector.dot(mY), worldVector.dot(mZ)};
}

Vec3D CoordinateFrame3D::transformVectorToWorld(const Vec3D& localVector) const noexcept {
    return {
        localVector.x() * mX.x() + localVector.y() * mY.x() + localVector.z() * mZ.x(),
        localVector.x() * mX.y() + localVector.y() * mY.y() + localVector.z() * mZ.y(),
        localVector.x() * mX.z() + localVector.y() * mY.z() + localVector.z() * mZ.z()
    };
}

CoordinateFrame3D CoordinateFrame3D::worldFrame() noexcept {
    return {};
}

CoordinateFrame3D CoordinateFrame3D::fromOriginAndZ(const Point3D& origin, const Vec3D& zAxis) {
    Vec3D z = zAxis.normalized();
    
    // Choose an arbitrary perpendicular vector
    const Vec3D arbitrary = std::abs(z.z()) < 0.9 ? Vec3D::unitZ() : Vec3D::unitX();
    Vec3D x = arbitrary.cross(z).normalized();
    Vec3D y = z.cross(x);
    
    return {origin, x, y, z};
}

} // namespace CwAPI3D::Geometry
