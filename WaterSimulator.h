#pragma once

#include "Vec3.h"
#include "Shader.h"
#include "WaterParticle.h"
#include "Geometry.h"

#include <iostream>
#include <vector>




class ParticleGrid {
private:
	// A one dimensional grid width*height*depth that contains for each tile a vector containing the water particles in this tile
	std::vector<WaterParticle*>* grid;
	int width;
	int height;
	int depth;
	float tile_size;

	// All the particles must be in the box defined by start - end
	Vec3 start;
	Vec3 end;

	// Give the index in the grid of a point in world position vec
	int vec2index(Vec3 vec);
	int vec2index_not_protected(Vec3 vec);

	// Give the position of the center of the index-th tile of the grid
	Vec3 index2vec(int index);

	int current_tile_number;     // The number between 0 to 26 describing
	int current_tile_index;      // The index in the grid of the current tile 
	int current_particle_index;  // The particle index in the list of particles in the tile

	void next_tile();
public:
	ParticleGrid() {}
	ParticleGrid(float tile_size, Vec3 start, Vec3 end);

	void setParticles(WaterParticle* particles, size_t nbParticles);
	// Get all the particles at a distance less than grid_size.
	std::vector<WaterParticle*> getNear(Vec3 point);

	// getNear is really slow. So, the user can iterate through the near waterParticles instead.
	// Starts the iteration, and then, loops doing next() until next() gives null.
	void startIterationNears(Vec3 point);
	WaterParticle* next();
};




class WaterSimulator {
private:
	WaterParticle *waterParticles;  // The array of particles
	size_t nbParticles;

	ParticleGrid particleGrid;  // The data structure for a faster look up

	Plan* plans;
	size_t nb_plans;

	float volume_inv;

public:
	static constexpr float DENSITY_RADIUS = 0.1f;

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
	Vec3 compute_pressure_force(WaterParticle p1, WaterParticle p2);

	void draw_density(Vec3 pos) { std::cout << "density at (" << pos.getX() << ' ' << pos.getY() << ' ' << pos.getZ() << "): " << compute_density(pos) << std::endl; }
	
};
