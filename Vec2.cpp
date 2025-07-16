#include "Vec2.h"
#include <cmath>

Vec2::Vec2(float x_, float y_) { x = x_; y = y_; }


float Vec2::getX() const { return x; }
float Vec2::getY() const { return y; }


Vec2 Vec2::operator+(Vec2 other) const { return Vec2(x + other.getX(), y + other.getY()); }
Vec2 Vec2::operator-() const { return Vec2(-x, -y); }
Vec2 Vec2::operator-(Vec2 other) const { return *this + (-other); }
Vec2 Vec2::operator*(Vec2 other) const  { return Vec2(x * other.getX(), y * other.getY()); }
Vec2 Vec2::operator*(float other) const { return Vec2(x * other, y * other); }
Vec2 Vec2::operator/(Vec2 other) const { return Vec2(x / other.getX(), y / other.getY()); }
Vec2 Vec2::operator/(float other) const { return Vec2(x / other, y / other); }


float Vec2::norm() const { return x * x + y * y; }
Vec2 Vec2::normalize() const { return *this / norm(); }
Vec2 Vec2::resize(float size) const { return normalize() * size; }


float dist(const Vec2 v1, const Vec2 v2) { return std::sqrt(dot(v1, v2)); }
float dot(const Vec2 v1, const Vec2 v2) { return v1.getX() * v2.getX() + v1.getY() * v2.getY(); }
float cross(const Vec2 v1, const Vec2 v2) { return (v1.getX() * v2.getY()) - (v1.getY() * v2.getX()); }
