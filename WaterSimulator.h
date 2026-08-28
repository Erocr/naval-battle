#pragma once

#include "Vec3.h"
#include "Shader.h"
#include "WaterParticle.h"
#include "Geometry.h"

#include <iostream>
#include <vector>

class WaterSimulator {
private:
	WaterParticle *waterParticles;  // The array of particles
	size_t nbParticles;

	Plan* plans;
	size_t nb_plans;

public:
	static const float DENSITY_RADIUS;

	WaterSimulator() {}
	WaterSimulator(size_t nbParticles);
	WaterSimulator(size_t nbParticles, float density_radius);

	static float smoothing_func(float d);
	static float smoothing_func_prime(float d);

	void update();
	void putOnShader(Shader sdfShader);
	void updateParticles();

	float compute_density(Vec3 pos);
	void computeCollisions();

	void cache_densities();
	Vec3 compute_pressure_force(size_t i, size_t j);

	void draw_density(Vec3 pos) { std::cout << "density at (" << pos.getX() << ' ' << pos.getY() << ' ' << pos.getZ() << "): " << compute_density(pos) << std::endl; }
	
};


