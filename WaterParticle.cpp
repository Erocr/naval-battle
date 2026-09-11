#include "WaterParticle.h"

#include <iostream>

const float WaterParticle::GRAVITY = 0.005;
const float WaterParticle::FRICTION = 0.98;


WaterParticle::WaterParticle(Vec3 pos) {
	position = pos;
	speed = Vec3(0, 0, 0);
	pressure_force = Vec3(0, 0, 0);

	mass = 1.0f;

	density = 0.0f;
	density_inv = 0.0f;
	pressure = 0.0f;
}

void WaterParticle::computePressure() {
	float TARGET_DENSITY = 1;
	float PRESSURE_MULTIPLIER = 0.02f;
	// The pressure is how much he must get close to other particles
	pressure = -(density - TARGET_DENSITY) * PRESSURE_MULTIPLIER;
}


void WaterParticle::update() {
	//std::cout << "pressure force : " << pressure_force.getX() << ' ' << pressure_force.getY() << ' ' << pressure_force.getZ() << std::endl;
	speed = speed * FRICTION;
	Vec3 forces = pressure_force + Vec3(0, -1, 0) * GRAVITY;
	speed += forces / mass;
	position += speed;
}
