#include "cwapi3d/geometry/Vec3D.h"
#include <cmath>
#include <limits>
#include <stdexcept>

namespace cwapi3d::geometry {

constexpr double EPSILON =  1e-6; // std::numeric_limits<double>::epsilon() * 100;

bool Vec3D::operator==(const Vec3D& other) const noexcept {
    return std::abs(mComponents[0] - other.mComponents[0]) < EPSILON &&
           std::abs(mComponents[1] - other.mComponents[1]) < EPSILON &&
           std::abs(mComponents[2] - other.mComponents[2]) < EPSILON;
}

bool Vec3D::operator!=(const Vec3D& other) const noexcept {
    return !(*this == other);
}

double Vec3D::length() const noexcept {
    return std::sqrt(lengthSquared());
}

double Vec3D::lengthSquared() const noexcept {
    return mComponents[0] * mComponents[0] +
           mComponents[1] * mComponents[1] +
           mComponents[2] * mComponents[2];
}

Vec3D Vec3D::normalized() const {
    const double len = length();
    if (len < EPSILON) {
        throw std::runtime_error("Cannot normalize zero-length vector");
    }
    return *this / len;
}

void Vec3D::normalize() {
    const double len = length();
    if (len < EPSILON) {
        throw std::runtime_error("Cannot normalize zero-length vector");
    }
    *this /= len;
}

double Vec3D::dot(const Vec3D& other) const noexcept {
    return mComponents[0] * other.mComponents[0] +
           mComponents[1] * other.mComponents[1] +
           mComponents[2] * other.mComponents[2];
}

Vec3D Vec3D::cross(const Vec3D& other) const noexcept {
    return Vec3D(
        mComponents[1] * other.mComponents[2] - mComponents[2] * other.mComponents[1],
        mComponents[2] * other.mComponents[0] - mComponents[0] * other.mComponents[2],
        mComponents[0] * other.mComponents[1] - mComponents[1] * other.mComponents[0]
    );
}

Vec3D Vec3D::operator+(const Vec3D& other) const noexcept {
    return Vec3D(mComponents[0] + other.mComponents[0],
                 mComponents[1] + other.mComponents[1],
                 mComponents[2] + other.mComponents[2]);
}

Vec3D Vec3D::operator-(const Vec3D& other) const noexcept {
    return Vec3D(mComponents[0] - other.mComponents[0],
                 mComponents[1] - other.mComponents[1],
                 mComponents[2] - other.mComponents[2]);
}

Vec3D Vec3D::operator*(const double scalar) const noexcept {
    return Vec3D(mComponents[0] * scalar,
                 mComponents[1] * scalar,
                 mComponents[2] * scalar);
}

Vec3D Vec3D::operator/(const double scalar) const {
    if (std::abs(scalar) < EPSILON) {
        throw std::runtime_error("Division by zero");
    }
    return Vec3D(mComponents[0] / scalar,
                 mComponents[1] / scalar,
                 mComponents[2] / scalar);
}

Vec3D Vec3D::operator-() const noexcept {
    return Vec3D(-mComponents[0], -mComponents[1], -mComponents[2]);
}

Vec3D& Vec3D::operator+=(const Vec3D& other) noexcept {
    mComponents[0] += other.mComponents[0];
    mComponents[1] += other.mComponents[1];
    mComponents[2] += other.mComponents[2];
    return *this;
}

Vec3D& Vec3D::operator-=(const Vec3D& other) noexcept {
    mComponents[0] -= other.mComponents[0];
    mComponents[1] -= other.mComponents[1];
    mComponents[2] -= other.mComponents[2];
    return *this;
}

Vec3D& Vec3D::operator*=(const double scalar) noexcept {
    mComponents[0] *= scalar;
    mComponents[1] *= scalar;
    mComponents[2] *= scalar;
    return *this;
}

Vec3D& Vec3D::operator/=(const double scalar) {
    if (std::abs(scalar) < EPSILON) {
        throw std::runtime_error("Division by zero");
    }
    mComponents[0] /= scalar;
    mComponents[1] /= scalar;
    mComponents[2] /= scalar;
    return *this;
}

Vec3D operator*(const double scalar, const Vec3D& vec) noexcept {
    return vec * scalar;
}

} // namespace cwapi3d::geometry
