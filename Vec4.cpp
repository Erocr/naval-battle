#include "Vec4.h"
#include <cmath>

Vec4::Vec4() { x = 0; y = 0; z = 0; w = 0; }
Vec4::Vec4(float x_, float y_, float z_, float w_) { x = x_; y = y_; z = z_; w = w_; }


float Vec4::getX() const { return x; }
float Vec4::getY() const { return y; }
float Vec4::getZ() const { return z; }
float Vec4::getW() const { return w; }


Vec4 Vec4::operator+(Vec4 other) const {
	return Vec4(x + other.getX(), y + other.getY(), z + other.getZ(), w + other.getW());
}
Vec4 Vec4::operator-() const {
	return Vec4(-x, -y, -z, -w);
}
Vec4 Vec4::operator-(Vec4 other) const {
	return *this + (-other);
}
Vec4 Vec4::operator*(Vec4 other) const {
	return Vec4(x * other.getX(), y * other.getY(), z * other.getZ(), w * other.getW());
}
Vec4 Vec4::operator*(float other) const {
	return Vec4(x * other, y * other, z * other, w * other);
}
Vec4 Vec4::operator/(Vec4 other) const {
	return Vec4(x / other.getX(), y / other.getY(), z / other.getZ(), w / other.getW());
}
Vec4 Vec4::operator/(float other) const {
	return Vec4(x / other, y / other, z / other, w / other);
}
void Vec4::operator+=(Vec4 other) {
	x = x + other.getX();
	y = y + other.getY();
	z = z + other.getZ();
	w = w + other.getW();
}


float Vec4::norm() const { return x * x + y * y + z * z + w * w; }
Vec4 Vec4::normalize() const { return *this / norm(); }
Vec4 Vec4::resize(float size) const { return normalize() * size; }


float dist(const Vec4 v1, const Vec4 v2) {
	return std::sqrt(dot(v1, v2));
}
float dot(const Vec4 v1, const Vec4 v2) {
	return v1.getX() * v2.getX() + v1.getY() * v2.getY() + v1.getZ() * v2.getZ() + v1.getW() * v2.getW();
}
