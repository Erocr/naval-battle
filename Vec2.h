#pragma once


class Vec2 {
private:
	float x, y;

public:
	Vec2(float x_, float y_);

	// getters
	float getX() const;
	float getY() const;


	// operators
	Vec2 operator+(Vec2 other) const;
	Vec2 operator-(Vec2 other) const;
	Vec2 operator-() const;
	Vec2 operator*(Vec2 other) const;
	Vec2 operator*(float other) const;
	Vec2 operator/(Vec2 other) const;
	Vec2 operator/(float other) const;


	// usefull functions
	float norm() const;
	Vec2 normalize() const;
	Vec2 resize(float size) const;
};


float dist(const Vec2 v1, const Vec2 v2);
float dot(const Vec2 v1, const Vec2 v2);
float cross(const Vec2 v1, const Vec2 v2);
