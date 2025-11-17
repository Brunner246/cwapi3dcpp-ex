#pragma once

#include <cwapi3d/geometry/geometry3d_export.h>

#include <array>
#include <cmath>

namespace cwapi3d::geometry {

/**
 * @brief Represents a 3D vector
 */
class GEOMETRY3D_EXPORT Vec3D {
   public:
    Vec3D() noexcept : mComponents{{0.0, 0.0, 0.0}} {}
    Vec3D(const double x, const double y, const double z) noexcept : mComponents{{x, y, z}} {}

    // Accessors
    [[nodiscard]] double x() const noexcept { return mComponents[0]; }
    [[nodiscard]] double y() const noexcept { return mComponents[1]; }
    [[nodiscard]] double z() const noexcept { return mComponents[2]; }

    void setX(const double x) noexcept { mComponents[0] = x; }
    void setY(const double y) noexcept { mComponents[1] = y; }
    void setZ(const double z) noexcept { mComponents[2] = z; }

    // Operators
    [[nodiscard]] double operator[](const size_t index) const { return mComponents.at(index); }
    double& operator[](const size_t index) { return mComponents.at(index); }

    [[nodiscard]] bool operator==(const Vec3D& other) const noexcept;
    [[nodiscard]] bool operator!=(const Vec3D& other) const noexcept;

    // Vector operations
    [[nodiscard]] double length() const noexcept;
    [[nodiscard]] double lengthSquared() const noexcept;
    [[nodiscard]] Vec3D normalized() const;
    void normalize();

    [[nodiscard]] double dot(const Vec3D& other) const noexcept;
    [[nodiscard]] Vec3D cross(const Vec3D& other) const noexcept;

    // Arithmetic operations
    [[nodiscard]] Vec3D operator+(const Vec3D& other) const noexcept;
    [[nodiscard]] Vec3D operator-(const Vec3D& other) const noexcept;
    [[nodiscard]] Vec3D operator*(double scalar) const noexcept;
    [[nodiscard]] Vec3D operator/(double scalar) const;
    [[nodiscard]] Vec3D operator-() const noexcept;

    Vec3D& operator+=(const Vec3D& other) noexcept;
    Vec3D& operator-=(const Vec3D& other) noexcept;
    Vec3D& operator*=(double scalar) noexcept;
    Vec3D& operator/=(double scalar);

    // Static utility vectors
    [[nodiscard]] static Vec3D unitX() noexcept { return {1.0, 0.0, 0.0}; }
    [[nodiscard]] static Vec3D unitY() noexcept { return {0.0, 1.0, 0.0}; }
    [[nodiscard]] static Vec3D unitZ() noexcept { return {0.0, 0.0, 1.0}; }
    [[nodiscard]] static Vec3D zero() noexcept { return {0.0, 0.0, 0.0}; }

   private:
    std::array<double, 3> mComponents;
};

[[nodiscard]] Vec3D operator*(double scalar, const Vec3D& vec) noexcept;

}  // namespace cwapi3d::geometry
