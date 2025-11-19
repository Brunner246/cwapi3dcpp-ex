#include "cwapi3d/geometry/Plane3D.h"

#include <cmath>
#include <stdexcept>

namespace CwAPI3D::Geometry {

Plane3D::Plane3D() noexcept : mOrigin(0.0, 0.0, 0.0), mNormal(0.0, 0.0, 1.0) {}

Plane3D::Plane3D(const Point3D& origin, const Vec3D& normal)
    : mOrigin(origin), mNormal(normal.normalized()) {}

void Plane3D::setNormal(const Vec3D& normal) { mNormal = normal.normalized(); }

double Plane3D::distanceToPoint(const Point3D& point) const noexcept {
    const Vec3D v(point.x() - mOrigin.x(), point.y() - mOrigin.y(), point.z() - mOrigin.z());
    return v.dot(mNormal);
}

Point3D Plane3D::projectPoint(const Point3D& point) const noexcept {
    const double distance = distanceToPoint(point);
    return {point.x() - distance * mNormal.x(), point.y() - distance * mNormal.y(),
            point.z() - distance * mNormal.z()};
}

bool Plane3D::isPointOnPlane(const Point3D& point, const double tolerance) const noexcept {
    return std::abs(distanceToPoint(point)) < tolerance;
}

Plane3D Plane3D::fromPoints(const Point3D& p1, const Point3D& p2, const Point3D& p3) {
    const Vec3D v1(p2.x() - p1.x(), p2.y() - p1.y(), p2.z() - p1.z());
    const Vec3D v2(p3.x() - p1.x(), p3.y() - p1.y(), p3.z() - p1.z());
    const Vec3D normal = v1.cross(v2);

    if (normal.lengthSquared() < 1e-12) {
        throw std::runtime_error("Points are collinear, cannot define a plane");
    }

    return {p1, normal};
}

Plane3D Plane3D::xyPlane() noexcept { return {Point3D(0.0, 0.0, 0.0), Vec3D::unitZ()}; }

Plane3D Plane3D::xzPlane() noexcept { return {Point3D(0.0, 0.0, 0.0), Vec3D::unitY()}; }

Plane3D Plane3D::yzPlane() noexcept { return {Point3D(0.0, 0.0, 0.0), Vec3D::unitX()}; }

}  // namespace CwAPI3D::Geometry
