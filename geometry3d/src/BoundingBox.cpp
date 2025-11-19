#include "cwapi3d/geometry/BoundingBox.h"

#include <algorithm>
#include <limits>

namespace cwapi3d::geometry {

// AABB Implementation

AABB::AABB() noexcept
    : m_min(std::numeric_limits<double>::max(), std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max()),
      m_max(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(),
            std::numeric_limits<double>::lowest()) {}

AABB::AABB(const Point3D& min, const Point3D& max) noexcept : m_min(min), m_max(max) {}

Point3D AABB::center() const noexcept {
    return {(m_min.x() + m_max.x()) * 0.5, (m_min.y() + m_max.y()) * 0.5,
            (m_min.z() + m_max.z()) * 0.5};
}

Vec3D AABB::size() const noexcept {
    return {m_max.x() - m_min.x(), m_max.y() - m_min.y(), m_max.z() - m_min.z()};
}

double AABB::volume() const noexcept {
    if (!isValid()) {
        return 0.0;
    }
    const Vec3D s = size();
    return s.x() * s.y() * s.z();
}

bool AABB::contains(const Point3D& point) const noexcept {
    return point.x() >= m_min.x() && point.x() <= m_max.x() && point.y() >= m_min.y() &&
           point.y() <= m_max.y() && point.z() >= m_min.z() && point.z() <= m_max.z();
}

bool AABB::intersects(const AABB& other) const noexcept {
    return !(m_max.x() < other.m_min.x() || m_min.x() > other.m_max.x() ||
             m_max.y() < other.m_min.y() || m_min.y() > other.m_max.y() ||
             m_max.z() < other.m_min.z() || m_min.z() > other.m_max.z());
}

void AABB::expand(const Point3D& point) noexcept {
    m_min.setX(std::min(m_min.x(), point.x()));
    m_min.setY(std::min(m_min.y(), point.y()));
    m_min.setZ(std::min(m_min.z(), point.z()));

    m_max.setX(std::max(m_max.x(), point.x()));
    m_max.setY(std::max(m_max.y(), point.y()));
    m_max.setZ(std::max(m_max.z(), point.z()));
}

void AABB::expand(const AABB& other) noexcept {
    expand(other.m_min);
    expand(other.m_max);
}

AABB AABB::merged(const AABB& other) const noexcept {
    AABB result = *this;
    result.expand(other);
    return result;
}

bool AABB::isValid() const noexcept {
    return m_min.x() <= m_max.x() && m_min.y() <= m_max.y() && m_min.z() <= m_max.z();
}

void AABB::reset() noexcept {
    m_min = Point3D(std::numeric_limits<double>::max(), std::numeric_limits<double>::max(),
                    std::numeric_limits<double>::max());
    m_max = Point3D(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(),
                    std::numeric_limits<double>::lowest());
}

// BoundingBox Implementation

BoundingBox::BoundingBox() noexcept
    : mCenter(0.0, 0.0, 0.0),
      mSize(0.0, 0.0, 0.0),
      mAxes{{Vec3D::unitX(), Vec3D::unitY(), Vec3D::unitZ()}} {}

BoundingBox::BoundingBox(const AABB& aabb) noexcept
    : mCenter(aabb.center()),
      mSize(aabb.size()),
      mAxes{{Vec3D::unitX(), Vec3D::unitY(), Vec3D::unitZ()}} {}

bool BoundingBox::contains(const Point3D& point) const noexcept {
    // Transform point to local space
    const Vec3D v(point.x() - mCenter.x(), point.y() - mCenter.y(), point.z() - mCenter.z());

    const Vec3D localPoint(v.dot(mAxes[0]), v.dot(mAxes[1]), v.dot(mAxes[2]));

    const Vec3D halfSize = mSize * 0.5;
    return std::abs(localPoint.x()) <= halfSize.x() && std::abs(localPoint.y()) <= halfSize.y() &&
           std::abs(localPoint.z()) <= halfSize.z();
}

AABB BoundingBox::toAABB() const noexcept {
    AABB result;

    // Transform 8 corners to world space and expand AABB
    const Vec3D halfSize = mSize * 0.5;

    for (int i = 0; i < 8; ++i) {
        const Vec3D corner((i & 1) ? halfSize.x() : -halfSize.x(),
                           (i & 2) ? halfSize.y() : -halfSize.y(),
                           (i & 4) ? halfSize.z() : -halfSize.z());

        const Point3D worldCorner(mCenter.x() + corner.x() * mAxes[0].x() +
                                      corner.y() * mAxes[1].x() + corner.z() * mAxes[2].x(),
                                  mCenter.y() + corner.x() * mAxes[0].y() +
                                      corner.y() * mAxes[1].y() + corner.z() * mAxes[2].y(),
                                  mCenter.z() + corner.x() * mAxes[0].z() +
                                      corner.y() * mAxes[1].z() + corner.z() * mAxes[2].z());

        result.expand(worldCorner);
    }

    return result;
}

// OBB Implementation

OBB::OBB() noexcept : mFrame(CoordinateFrame3D::worldFrame()), mHalfExtents(0.0, 0.0, 0.0) {}

OBB::OBB(const CoordinateFrame3D& frame, const Vec3D& halfExtents) noexcept
    : mFrame(frame), mHalfExtents(halfExtents) {}

OBB::OBB(const Point3D& center, const Vec3D& xAxis, const Vec3D& yAxis, const Vec3D& zAxis,
         const Vec3D& halfExtents)
    : mFrame(center, xAxis, yAxis, zAxis), mHalfExtents(halfExtents) {}

OBB OBB::fromAABB(const AABB& aabb, const CoordinateFrame3D& frame) noexcept {
    const Vec3D size = aabb.size();
    const Vec3D halfExtents = size * 0.5;

    // Create OBB with the same center as AABB but with the provided frame orientation
    CoordinateFrame3D obbFrame = frame;
    obbFrame.setOrigin(aabb.center());

    return {obbFrame, halfExtents};
}

OBB OBB::fromPoints(const std::vector<Point3D>& points, const CoordinateFrame3D& frame) {
    if (points.empty()) {
        return {};
    }

    // Transform all points to local coordinate system
    Vec3D minLocal(std::numeric_limits<double>::max(), std::numeric_limits<double>::max(),
                   std::numeric_limits<double>::max());
    Vec3D maxLocal(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(),
                   std::numeric_limits<double>::lowest());

    for (const auto& point : points) {
        const Point3D localPoint = frame.transformToLocal(point);

        minLocal.setX(std::min(minLocal.x(), localPoint.x()));
        minLocal.setY(std::min(minLocal.y(), localPoint.y()));
        minLocal.setZ(std::min(minLocal.z(), localPoint.z()));

        maxLocal.setX(std::max(maxLocal.x(), localPoint.x()));
        maxLocal.setY(std::max(maxLocal.y(), localPoint.y()));
        maxLocal.setZ(std::max(maxLocal.z(), localPoint.z()));
    }

    // Calculate center in local space and then transform back to world
    const Point3D localCenter((minLocal.x() + maxLocal.x()) * 0.5,
                              (minLocal.y() + maxLocal.y()) * 0.5,
                              (minLocal.z() + maxLocal.z()) * 0.5);

    const Point3D worldCenter = frame.transformToWorld(localCenter);
    const Vec3D halfExtents((maxLocal.x() - minLocal.x()) * 0.5,
                            (maxLocal.y() - minLocal.y()) * 0.5,
                            (maxLocal.z() - minLocal.z()) * 0.5);

    CoordinateFrame3D obbFrame = frame;
    obbFrame.setOrigin(worldCenter);

    return {obbFrame, halfExtents};
}

OBB OBB::fromCenterAndSize(const Point3D& center, const Vec3D& size,
                           const CoordinateFrame3D& frame) noexcept {
    CoordinateFrame3D obbFrame = frame;
    obbFrame.setOrigin(center);

    const Vec3D halfExtents = size * 0.5;
    return {obbFrame, halfExtents};
}

Vec3D OBB::size() const noexcept { return mHalfExtents * 2.0; }

double OBB::volume() const noexcept {
    const Vec3D s = size();
    return s.x() * s.y() * s.z();
}

std::array<Point3D, 8> OBB::getCorners() const noexcept {
    std::array<Point3D, 8> corners;

    // Generate all 8 corners in local space, then transform to world
    for (int i = 0; i < 8; ++i) {
        const Point3D localCorner((i & 1) ? mHalfExtents.x() : -mHalfExtents.x(),
                                  (i & 2) ? mHalfExtents.y() : -mHalfExtents.y(),
                                  (i & 4) ? mHalfExtents.z() : -mHalfExtents.z());

        corners[i] = mFrame.transformToWorld(localCorner);
    }

    return corners;
}

bool OBB::contains(const Point3D& point) const noexcept {
    // Transform point to local coordinate system
    const Point3D localPoint = mFrame.transformToLocal(point);

    // Check if point is within half extents in local space
    return std::abs(localPoint.x()) <= mHalfExtents.x() &&
           std::abs(localPoint.y()) <= mHalfExtents.y() &&
           std::abs(localPoint.z()) <= mHalfExtents.z();
}

bool OBB::intersects(const OBB& other) const noexcept {
    // Use Separating Axis Theorem (SAT) for OBB-OBB intersection
    // Test all 15 potential separating axes:
    // - 3 face normals from this OBB
    // - 3 face normals from other OBB
    // - 9 cross products of edges

    const Vec3D axes[15] = {
        // Face normals of this OBB
        mFrame.xAxis(), mFrame.yAxis(), mFrame.zAxis(),
        // Face normals of other OBB
        other.mFrame.xAxis(), other.mFrame.yAxis(), other.mFrame.zAxis(),
        // Cross products of edges
        mFrame.xAxis().cross(other.mFrame.xAxis()), mFrame.xAxis().cross(other.mFrame.yAxis()),
        mFrame.xAxis().cross(other.mFrame.zAxis()), mFrame.yAxis().cross(other.mFrame.xAxis()),
        mFrame.yAxis().cross(other.mFrame.yAxis()), mFrame.yAxis().cross(other.mFrame.zAxis()),
        mFrame.zAxis().cross(other.mFrame.xAxis()), mFrame.zAxis().cross(other.mFrame.yAxis()),
        mFrame.zAxis().cross(other.mFrame.zAxis())};

    const Vec3D centerDiff(other.center().x() - center().x(), other.center().y() - center().y(),
                           other.center().z() - center().z());

    if (!std::ranges::all_of(axes, [&](const Vec3D& lAxis) {
            // Treat degenerate axes as non-separating
            if (lAxis.lengthSquared() < 1e-10) {
                return true;
            }
            const Vec3D lNormalizedAxis = lAxis.normalized();
            const double lThisProjection = projectOntoAxis(lNormalizedAxis);
            const double lOtherProjection = other.projectOntoAxis(lNormalizedAxis);
            const double lCenterDistance = std::abs(centerDiff.dot(lNormalizedAxis));
            return lCenterDistance <= lThisProjection + lOtherProjection;
        })) {
        return false;
    }

    // No separating axis found, OBBs intersect
    return true;
}

bool OBB::intersects(const AABB& aabb) const noexcept {
    // Convert AABB to OBB and use OBB-OBB intersection
    const OBB aabbAsOBB = OBB::fromAABB(aabb);
    return intersects(aabbAsOBB);
}

void OBB::expand(const Point3D& point) noexcept {
    // Transform point to local space
    const Point3D localPoint = mFrame.transformToLocal(point);

    // Expand half extents if needed
    mHalfExtents.setX(std::max(mHalfExtents.x(), std::abs(localPoint.x())));
    mHalfExtents.setY(std::max(mHalfExtents.y(), std::abs(localPoint.y())));
    mHalfExtents.setZ(std::max(mHalfExtents.z(), std::abs(localPoint.z())));
}

void OBB::expand(const OBB& other) noexcept {
    // Expand to include all corners of the other OBB
    for (const auto corners = other.getCorners(); const auto& corner : corners) {
        expand(corner);
    }
}

OBB OBB::merged(const OBB& other) const noexcept {
    OBB result = *this;
    result.expand(other);
    return result;
}

AABB OBB::toAABB() const noexcept {
    AABB result;

    // Expand AABB to include all 8 corners
    const auto corners = getCorners();
    for (const auto& corner : corners) {
        result.expand(corner);
    }

    return result;
}

bool OBB::isValid() const noexcept {
    return mHalfExtents.x() >= 0.0 && mHalfExtents.y() >= 0.0 && mHalfExtents.z() >= 0.0;
}

double OBB::projectOntoAxis(const Vec3D& axis) const noexcept {
    // Project the half extents onto the axis
    return mHalfExtents.x() * std::abs(mFrame.xAxis().dot(axis)) +
           mHalfExtents.y() * std::abs(mFrame.yAxis().dot(axis)) +
           mHalfExtents.z() * std::abs(mFrame.zAxis().dot(axis));
}

}  // namespace cwapi3d::geometry
