#pragma once


class Vec3 {
private:
	float x, y, z;

public:
	Vec3(float x_, float y_, float z_);

	// getters
	float getX() const;
	float getY() const;
	float getZ() const;


	// operators
	Vec3 operator+(Vec3 other) const;
	Vec3 operator-(Vec3 other) const;
	Vec3 operator-() const;
	Vec3 operator*(Vec3 other) const;
	Vec3 operator*(float other) const;
	Vec3 operator/(Vec3 other) const;
	Vec3 operator/(float other) const;


	// usefull functions
	float norm() const;
	Vec3 normalize() const;
	Vec3 resize(float size) const;
};


float dist(const Vec3 v1, const Vec3 v2);
float dot(const Vec3 v1, const Vec3 v2);
Vec3 cross(const Vec3 v1, const Vec3 v2);
