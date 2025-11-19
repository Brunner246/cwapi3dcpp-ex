#pragma once

#include <cwapi3d/geometry/geometry3d_export.h>

#include <vector>

#include "CoordinateFrame3D.h"
#include "Point3D.h"
#include "Vec3D.h"

namespace cwapi3d::geometry {

/**
 * @brief Axis-Aligned Bounding Box
 */
class GEOMETRY3D_EXPORT AABB {
   public:
    AABB() noexcept;
    AABB(const Point3D& min, const Point3D& max) noexcept;

    [[nodiscard]] const Point3D& min() const noexcept { return m_min; }
    [[nodiscard]] const Point3D& max() const noexcept { return m_max; }
    [[nodiscard]] Point3D center() const noexcept;
    [[nodiscard]] Vec3D size() const noexcept;
    [[nodiscard]] double volume() const noexcept;

    void setMin(const Point3D& min) noexcept { m_min = min; }
    void setMax(const Point3D& max) noexcept { m_max = max; }

    [[nodiscard]] bool contains(const Point3D& point) const noexcept;
    [[nodiscard]] bool intersects(const AABB& other) const noexcept;
    void expand(const Point3D& point) noexcept;
    void expand(const AABB& other) noexcept;
    [[nodiscard]] AABB merged(const AABB& other) const noexcept;

    [[nodiscard]] bool isValid() const noexcept;
    void reset() noexcept;

   private:
    Point3D m_min;
    Point3D m_max;
};

/**
 * @brief General bounding box (non-axis-aligned)
 */
class GEOMETRY3D_EXPORT BoundingBox {
   public:
    BoundingBox() noexcept;
    explicit BoundingBox(const AABB& aabb) noexcept;

    [[nodiscard]] Point3D center() const noexcept { return mCenter; }
    [[nodiscard]] const Vec3D& size() const noexcept { return mSize; }
    [[nodiscard]] const std::array<Vec3D, 3>& axes() const noexcept { return mAxes; }

    [[nodiscard]] bool contains(const Point3D& point) const noexcept;
    [[nodiscard]] AABB toAABB() const noexcept;

   private:
    Point3D mCenter;
    Vec3D mSize;
    std::array<Vec3D, 3> mAxes;
};

/**
 * @brief Oriented Bounding Box aligned to a reference coordinate system
 *
 * This class represents an oriented bounding box (OBB) that uses a coordinate
 * frame to define its orientation. This is useful for representing bounding boxes
 * aligned to element coordinate systems or other custom reference frames.
 */
class GEOMETRY3D_EXPORT OBB {
   public:
    OBB() noexcept;
    OBB(const CoordinateFrame3D& frame, const Vec3D& halfExtents) noexcept;
    OBB(const Point3D& center, const Vec3D& xAxis, const Vec3D& yAxis, const Vec3D& zAxis,
        const Vec3D& halfExtents);

    [[nodiscard]] static OBB fromAABB(
        const AABB& aabb,
        const CoordinateFrame3D& frame = CoordinateFrame3D::worldFrame()) noexcept;
    [[nodiscard]] static OBB fromPoints(
        const std::vector<Point3D>& points,
        const CoordinateFrame3D& frame = CoordinateFrame3D::worldFrame());
    [[nodiscard]] static OBB fromCenterAndSize(
        const Point3D& center, const Vec3D& size,
        const CoordinateFrame3D& frame = CoordinateFrame3D::worldFrame()) noexcept;

    [[nodiscard]] const CoordinateFrame3D& frame() const noexcept { return mFrame; }
    [[nodiscard]] const Vec3D& halfExtents() const noexcept { return mHalfExtents; }
    [[nodiscard]] Point3D center() const noexcept { return mFrame.origin(); }
    [[nodiscard]] Vec3D size() const noexcept;
    [[nodiscard]] double volume() const noexcept;

    void setFrame(const CoordinateFrame3D& frame) noexcept { mFrame = frame; }
    void setHalfExtents(const Vec3D& halfExtents) noexcept { mHalfExtents = halfExtents; }

    [[nodiscard]] std::array<Point3D, 8> getCorners() const noexcept;

    [[nodiscard]] bool contains(const Point3D& point) const noexcept;
    [[nodiscard]] bool intersects(const OBB& other) const noexcept;
    [[nodiscard]] bool intersects(const AABB& aabb) const noexcept;

    void expand(const Point3D& point) noexcept;
    void expand(const OBB& other) noexcept;
    [[nodiscard]] OBB merged(const OBB& other) const noexcept;

    [[nodiscard]] AABB toAABB() const noexcept;

    [[nodiscard]] bool isValid() const noexcept;

   private:
    CoordinateFrame3D mFrame;
    Vec3D mHalfExtents;

    [[nodiscard]] double projectOntoAxis(const Vec3D& axis) const noexcept;
};

}  // namespace cwapi3d::geometry
