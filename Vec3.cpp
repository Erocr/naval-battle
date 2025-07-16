#include "Vec3.h"
#include <cmath>

Vec3::Vec3(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }


float Vec3::getX() const { return x; }
float Vec3::getY() const { return y; }
float Vec3::getZ() const { return z; }


Vec3 Vec3::operator+(Vec3 other) const { 
	return Vec3(x + other.getX(), y + other.getY(), z + other.getZ()); 
}
Vec3 Vec3::operator-() const { 
	return Vec3(-x, -y, -z); 
}
Vec3 Vec3::operator-(Vec3 other) const { 
	return *this + (-other); 
}
Vec3 Vec3::operator*(Vec3 other) const { 
	return Vec3(x * other.getX(), y * other.getY(), z * other.getZ()); 
}
Vec3 Vec3::operator*(float other) const { 
	return Vec3(x * other, y * other, z * other); 
}
Vec3 Vec3::operator/(Vec3 other) const { 
	return Vec3(x / other.getX(), y / other.getY(), z / other.getZ()); 
}
Vec3 Vec3::operator/(float other) const { 
	return Vec3(x / other, y / other, z / other); 
}


float Vec3::norm() const { return x * x + y * y + z * z; }
Vec3 Vec3::normalize() const { return *this / norm(); }
Vec3 Vec3::resize(float size) const { return normalize() * size; }


float dist(const Vec3 v1, const Vec3 v2) { 
	return std::sqrt(dot(v1, v2)); 
}
float dot(const Vec3 v1, const Vec3 v2) { 
	return v1.getX() * v2.getX() + v1.getY() * v2.getY() + v1.getZ() * v2.getZ(); 
}
Vec3 cross(const Vec3 v1, const Vec3 v2) { 
	return Vec3(
		v1.getY() * v2.getZ() - v1.getZ() * v2.getY(),
		v1.getZ() * v2.getX() - v1.getX() * v2.getZ(),
		v1.getX() * v2.getY() - v1.getY() * v2.getX()
	); 
}
