#include "cwapi3d/geometry/Point3D.h"
#include "cwapi3d/geometry/Vec3D.h"

#include <cmath>
#include <limits>
#include <stdexcept>

namespace CwAPI3D::Geometry {

constexpr double EPSILON =  1e-6; // std::numeric_limits<double>::epsilon() * 100;

bool Point3D::operator==(const Point3D& other) const noexcept {
    return std::abs(mCoords[0] - other.mCoords[0]) < EPSILON &&
           std::abs(mCoords[1] - other.mCoords[1]) < EPSILON &&
           std::abs(mCoords[2] - other.mCoords[2]) < EPSILON;
}

bool Point3D::operator!=(const Point3D& other) const noexcept { return !(*this == other); }

double Point3D::distanceTo(const Point3D& other) const noexcept {
    return std::sqrt(distanceSquaredTo(other));
}

double Point3D::distanceSquaredTo(const Point3D& other) const noexcept {
    const double dx = mCoords[0] - other.mCoords[0];
    const double dy = mCoords[1] - other.mCoords[1];
    const double dz = mCoords[2] - other.mCoords[2];
    return dx * dx + dy * dy + dz * dz;
}

Point3D Point3D::operator+(const Point3D& other) const noexcept {
    return {mCoords[0] + other.mCoords[0], mCoords[1] + other.mCoords[1],
            mCoords[2] + other.mCoords[2]};
}

Point3D Point3D::operator+(const Vec3D& vec) const noexcept {
    return {mCoords[0] + vec.x(), mCoords[1] + vec.y(), mCoords[2] + vec.z()};
}

Point3D Point3D::operator-(const Vec3D& vec) const noexcept {
    return {mCoords[0] - vec.x(), mCoords[1] - vec.y(), mCoords[2] - vec.z()};
}

Vec3D Point3D::operator-(const Point3D& other) const noexcept {
    return {mCoords[0] - other.mCoords[0], mCoords[1] - other.mCoords[1],
            mCoords[2] - other.mCoords[2]};
}

Point3D Point3D::operator*(const double scalar) const noexcept {
    return {mCoords[0] * scalar, mCoords[1] * scalar, mCoords[2] * scalar};
}

Point3D Point3D::operator/(const double scalar) const {
    if (std::abs(scalar) < EPSILON) {
        throw std::runtime_error("Division by zero");
    }
    return {mCoords[0] / scalar, mCoords[1] / scalar, mCoords[2] / scalar};
}

Point3D& Point3D::operator+=(const Point3D& other) noexcept {
    mCoords[0] += other.mCoords[0];
    mCoords[1] += other.mCoords[1];
    mCoords[2] += other.mCoords[2];
    return *this;
}

Point3D& Point3D::operator-=(const Point3D& other) noexcept {
    mCoords[0] -= other.mCoords[0];
    mCoords[1] -= other.mCoords[1];
    mCoords[2] -= other.mCoords[2];
    return *this;
}

Point3D& Point3D::operator+=(const Vec3D& vec) noexcept {
    mCoords[0] += vec.x();
    mCoords[1] += vec.y();
    mCoords[2] += vec.z();
    return *this;
}

Point3D& Point3D::operator-=(const Vec3D& vec) noexcept {
    mCoords[0] -= vec.x();
    mCoords[1] -= vec.y();
    mCoords[2] -= vec.z();
    return *this;
}

Point3D& Point3D::operator*=(const double scalar) noexcept {
    mCoords[0] *= scalar;
    mCoords[1] *= scalar;
    mCoords[2] *= scalar;
    return *this;
}

Point3D& Point3D::operator/=(const double scalar) {
    if (std::fabs(scalar) < EPSILON) {
        throw std::runtime_error("Division by zero");
    }
    mCoords[0] /= scalar;
    mCoords[1] /= scalar;
    mCoords[2] /= scalar;
    return *this;
}

Point3D operator*(const double scalar, const Point3D& point) noexcept { return point * scalar; }

Point3D operator+(const Vec3D& vec, const Point3D& point) noexcept { return point + vec; }

}  // namespace CwAPI3D::Geometry
