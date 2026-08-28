#include "WaterSimulator.h"

#include <iostream>

const float WaterSimulator::DENSITY_RADIUS = 2;


WaterSimulator::WaterSimulator(size_t nbParticles) {
	this->nbParticles = nbParticles;
	waterParticles = (WaterParticle*)malloc(sizeof(WaterParticle) * nbParticles);

	// The particules spawn in the square of size 1x1x1 centered in (0, 0, 0)
	int side_size = std::ceil(std::cbrt(nbParticles));
	float grid_size = 1.0f / side_size;
	size_t i = 0;
	Vec3 mid = Vec3(0, 0, 0);
	for (float x = grid_size*0.5; x < 1; x += grid_size) {
		for (float y = grid_size * 0.5; y < 1; y += grid_size) {
			for (float z = grid_size * 0.5; z < 1; z += grid_size) {
				if (i >= nbParticles) break;
				waterParticles[i] = WaterParticle(Vec3(x-0.5f, y-0.5f, z-0.5f));
				mid += Vec3(x - 0.5f, y - 0.5f, z - 0.5f);
				i++;
			}
		}
	}
	std::cout << mid.getX();

	// Définitions of boundaries
	plans = (Plan*) malloc(sizeof(Plan) * 6);
	// If the point is toward the normal, the point is in the boundarie
	plans[0] = Plan{ Vec3(-2, 0, 0), Vec3(1, 0, 0) };
	plans[1] = Plan{ Vec3(0, 2, 0), Vec3(0, -1, 0) };
	plans[2] = Plan{ Vec3(0, 0, -1), Vec3(0, 0, 1) };
	plans[3] = Plan{ Vec3(2, 0, 0), Vec3(-1, 0, 0) };
	plans[4] = Plan{ Vec3(0, -2, 0), Vec3(0, 1, 0) };
	plans[5] = Plan{ Vec3(0, 0, 2), Vec3(0, 0, -1) };
	nb_plans = 6;
}


void WaterSimulator::update() {
	computeCollisions();

	for (int i = 0; i < nbParticles; i++) {
		waterParticles[i].setDensity(compute_density(waterParticles[i].getPos()));
		waterParticles[i].setPressureForce(Vec3(0, 0, 0));
		waterParticles[i].computePressure();
	}


	for (int i = 0; i < nbParticles; i++) {
		for (int j = 0; j < nbParticles; j++) {
			if (i != j) {
				Vec3 currentPressure = waterParticles[i].getPressureForce();
				Vec3 to_add = compute_pressure_force(i, j);
				waterParticles[i].setPressureForce(currentPressure + to_add);
			}
		}
	}
	updateParticles();

	Vec3 pos = waterParticles[0].getPos();
}

void WaterSimulator::updateParticles() {
	for (int i = 0; i < nbParticles; i++) {
		waterParticles[i].update();
	}
}


void WaterSimulator::putOnShader(Shader sdfShader) {
	sdfShader.activate();

	sdfShader.putUniform("nbWaterParticles", int(nbParticles));
	std::vector<Vec3> waterParticlesPos = std::vector<Vec3>(nbParticles);
	for (size_t i = 0; i < nbParticles; i++) {
		waterParticlesPos[i] = waterParticles[i].getPos();
	}
	sdfShader.putUniform("waterParticles", waterParticlesPos);
}


float WaterSimulator::smoothing_func(float d) {
	// We chosed f(x) = exp(-x) - exp(-r) So :
	// - the function is spiky in 0
	// - f(DENSITY_RADIUS) = 0
	if (d > DENSITY_RADIUS)
		return 0;
	return std::exp(-d) - std::exp(-DENSITY_RADIUS);
}


float WaterSimulator::smoothing_func_prime(float d) {
	if (d > DENSITY_RADIUS)
		return 0;
	return -std::exp(-d);
}


float WaterSimulator::compute_density(Vec3 pos) {
	float res = 0;
	for (int i = 0; i < nbParticles; i++) {
		// Instead of using the position, I use the position + speed, it gives a more stable solution.
		Vec3 particlePos = waterParticles[i].getPos() + waterParticles[i].getSpeed();
		float d = dist(particlePos, pos);
		res += smoothing_func(d) * waterParticles[i].getMass();
	}
	// It is the integral of the smoothing function on the sphere
	// It is equal to 4 * pi^2 * (-exp(-r) * (r+1) + 1)
	float volume = 39.47841 * (-std::exp(-DENSITY_RADIUS) * (DENSITY_RADIUS + 1) + 1);
	return res / volume;
}


Vec3 WaterSimulator::compute_pressure_force(size_t i, size_t j) {
	WaterParticle particle1 = waterParticles[i];
	WaterParticle particle2 = waterParticles[j];

	float mass = particle1.getMass();
	float dst = dist(particle1.getPos(), particle2.getPos());
	float slope = smoothing_func_prime(dst);
	float pressure = (particle1.getPressure() + particle2.getPressure()) * 0.5f;
	float density_inv = particle2.getDensityInv();
	Vec3 dir = Vec3(0, 1, 0);
	if (dst != 0)
		dir = (particle2.getPos() - particle1.getPos()) / dst;
	float magnitude = slope * mass * pressure * density_inv;
	return -dir * magnitude;
}


void WaterSimulator::computeCollisions() {
	float COLLISION_DAMPLING = 0.8;
	for (int j = 0; j < nbParticles; j++) {
		for (int i = 0; i < nb_plans; i++) {
			Plan plan = plans[i];
			if (dot(waterParticles[j].getPos() - plan.point, plan.normal) < 0) {
				// u is the orthogonal projection of the particle's speed on the line described by the normal of the plane
				Vec3 u = plan.normal * dot(waterParticles[j].getSpeed(), plan.normal);
				// Modify the velocity of the particle so that it bounces on the wall
				waterParticles[j].setSpeed(waterParticles[j].getSpeed() - u * (1 + COLLISION_DAMPLING));

				// u is now the orthogonal projection of how off boundarie the particle is on the normal of the plane
				u = plan.normal * dot(waterParticles[j].getPos() - plan.point, plan.normal);
				// We offset the particle so that is is in the boundarie
				Vec3 new_pos = waterParticles[j].getPos() - u;
				waterParticles[j].setPos(new_pos);
			}
		}
	}
}













