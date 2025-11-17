#pragma once

#include <cwapi3d/geometry/geometry3d_export.h>
#include "Point3D.h"
#include "Vec3D.h"
#include <array>

namespace cwapi3d::geometry {

/**
 * @brief Represents a 3D coordinate frame with origin and three orthonormal axes
 */
class GEOMETRY3D_EXPORT CoordinateFrame3D {
public:
    CoordinateFrame3D() noexcept;
    CoordinateFrame3D(const Point3D& origin, const Vec3D& xAxis, const Vec3D& yAxis, const Vec3D& zAxis);

    [[nodiscard]] const Point3D& origin() const noexcept { return mOrigin; }
    [[nodiscard]] const Vec3D& xAxis() const noexcept { return mX; }
    [[nodiscard]] const Vec3D& yAxis() const noexcept { return mY; }
    [[nodiscard]] const Vec3D& zAxis() const noexcept { return mZ; }

    void setOrigin(const Point3D& origin) noexcept { mOrigin = origin; }
    void setAxes(const Vec3D& xAxis, const Vec3D& yAxis, const Vec3D& zAxis);

    [[nodiscard]] Point3D transformToLocal(const Point3D& worldPoint) const noexcept;
    [[nodiscard]] Point3D transformToWorld(const Point3D& localPoint) const noexcept;
    
    [[nodiscard]] Vec3D transformVectorToLocal(const Vec3D& worldVector) const noexcept;
    [[nodiscard]] Vec3D transformVectorToWorld(const Vec3D& localVector) const noexcept;

    [[nodiscard]] static CoordinateFrame3D worldFrame() noexcept;
    [[nodiscard]] static CoordinateFrame3D fromOriginAndZ(const Point3D& origin, const Vec3D& zAxis);

private:
    Point3D mOrigin;
    Vec3D mX;
    Vec3D mY;
    Vec3D mZ;

    void orthonormalize();
};

} // namespace cwapi3d::geometry
