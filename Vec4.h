#pragma once


class Vec4 {
private:
	float x, y, z, w;

public:
	Vec4();
	Vec4(float x_, float y_, float z_, float w_);

	// getters
	float getX() const;
	float getY() const;
	float getZ() const;
	float getW() const;


	// operators
	Vec4 operator+(Vec4 other) const;
	Vec4 operator-(Vec4 other) const;
	Vec4 operator-() const;
	Vec4 operator*(Vec4 other) const;
	Vec4 operator*(float other) const;
	Vec4 operator/(Vec4 other) const;
	Vec4 operator/(float other) const;
	void operator+=(Vec4 other);


	// usefull functions
	float norm() const;
	Vec4 normalize() const;
	Vec4 resize(float size) const;
};


float dist(const Vec4 v1, const Vec4 v2);
float dot(const Vec4 v1, const Vec4 v2);
