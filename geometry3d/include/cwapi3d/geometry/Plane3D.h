#pragma once

#include <cwapi3d/geometry/geometry3d_export.h>
#include "Point3D.h"
#include "Vec3D.h"

namespace cwapi3d::geometry {

/**
 * @brief Represents a plane in 3D space defined by a point and a normal vector
 */
class GEOMETRY3D_EXPORT Plane3D {
public:
    Plane3D() noexcept;
    Plane3D(const Point3D& origin, const Vec3D& normal);

    [[nodiscard]] const Point3D& origin() const noexcept { return mOrigin; }
    [[nodiscard]] const Vec3D& normal() const noexcept { return mNormal; }

    void setOrigin(const Point3D& origin) noexcept { mOrigin = origin; }
    void setNormal(const Vec3D& normal);

    [[nodiscard]] double distanceToPoint(const Point3D& point) const noexcept;
    [[nodiscard]] Point3D projectPoint(const Point3D& point) const noexcept;
    [[nodiscard]] bool isPointOnPlane(const Point3D& point, double tolerance = 1e-9) const noexcept;

    [[nodiscard]] static Plane3D fromPoints(const Point3D& p1, const Point3D& p2, const Point3D& p3);
    [[nodiscard]] static Plane3D xyPlane() noexcept;
    [[nodiscard]] static Plane3D xzPlane() noexcept;
    [[nodiscard]] static Plane3D yzPlane() noexcept;

private:
    Point3D mOrigin;
    Vec3D mNormal;
};

} // namespace cwapi3d::geometry
