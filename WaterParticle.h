#pragma once

#include "Vec3.h"

class WaterParticle {
private:
	Vec3 position;
	Vec3 speed;
	Vec3 pressure_force;

	float mass;

	float density;
	float density_inv;
	float pressure;

public:
	static const float GRAVITY;
	static const float FRICTION;

	WaterParticle(Vec3 pos);

	void computePressure();  // Cache the pressure

	void update();

	float getMass() { return mass; }
	Vec3 getPos() { return position; }
	Vec3 getSpeed() { return speed; }
	Vec3 getPressureForce() { return pressure_force; }
	float getDensity() { return density; }
	float getDensityInv() { return density_inv; }
	float getPressure() { return pressure; }

	void setDensity(float new_density) { density = new_density; density_inv = 1 / density; }
	void setPressureForce(Vec3 new_force) { pressure_force = new_force; }
	void setSpeed(Vec3 new_speed) { speed = new_speed; }
	void setPos(Vec3 new_pos) { position = new_pos; }
};