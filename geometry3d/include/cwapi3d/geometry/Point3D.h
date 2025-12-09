#pragma once

#include <cwapi3d/geometry/geometry3d_export.h>
#include <cmath>
#include <array>

namespace CwAPI3D::Geometry {

            // Forward declaration
class Vec3D;

/**
 * @brief Represents a point in 3D space
 */
class GEOMETRY3D_EXPORT Point3D {
public:
    Point3D() noexcept : mCoords{{0.0, 0.0, 0.0}} {}
    Point3D(const double x, const double y, const double z) noexcept : mCoords{{x, y, z}} {}

    // Accessors
    [[nodiscard]] double x() const noexcept { return mCoords[0]; }
    [[nodiscard]] double y() const noexcept { return mCoords[1]; }
    [[nodiscard]] double z() const noexcept { return mCoords[2]; }

    void setX(const double x) noexcept { mCoords[0] = x; }
    void setY(const double y) noexcept { mCoords[1] = y; }
    void setZ(const double z) noexcept { mCoords[2] = z; }

    // Operators
    [[nodiscard]] double operator[](const size_t index) const { return mCoords[index]; }
    double& operator[](const size_t index) { return mCoords[index]; }

    [[nodiscard]] bool operator==(const Point3D& other) const noexcept;
    [[nodiscard]] bool operator!=(const Point3D& other) const noexcept;

    // Distance calculations
    [[nodiscard]] double distanceTo(const Point3D& other) const noexcept;
    [[nodiscard]] double distanceSquaredTo(const Point3D& other) const noexcept;

    // Arithmetic operations (Point with Point - kept for compatibility but semantically unclear)
    [[nodiscard]] Point3D operator+(const Point3D& other) const noexcept;
    [[nodiscard]] Point3D operator*(double scalar) const noexcept;
    [[nodiscard]] Point3D operator/(double scalar) const;

    Point3D& operator+=(const Point3D& other) noexcept;
    Point3D& operator-=(const Point3D& other) noexcept;
    Point3D& operator*=(double scalar) noexcept;
    Point3D& operator/=(double scalar);

    // Vector arithmetic operations (semantically correct point-vector operations)
    [[nodiscard]] Point3D operator+(const Vec3D& vec) const noexcept;
    [[nodiscard]] Point3D operator-(const Vec3D& vec) const noexcept;
    [[nodiscard]] Vec3D operator-(const Point3D& other) const noexcept;  // Displacement vector

    Point3D& operator+=(const Vec3D& vec) noexcept;
    Point3D& operator-=(const Vec3D& vec) noexcept;

private:
    std::array<double, 3> mCoords;
};

[[nodiscard]] Point3D operator*(double scalar, const Point3D& point) noexcept;
[[nodiscard]] Point3D operator+(const Vec3D& vec, const Point3D& point) noexcept;

} // namespace CwAPI3D::Geometry
