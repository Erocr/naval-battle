#include "WaterSimulator.h"

#include <iostream>


WaterSimulator::WaterSimulator(size_t nbParticles) {
	this->nbParticles = nbParticles;
	waterParticles = (WaterParticle*)malloc(sizeof(WaterParticle) * nbParticles);

	// The particules spawn in the square of size 1x1x1 centered in (0, 0, 0)
	int side_size = std::ceil(std::cbrt(nbParticles));
	float grid_size = 1.0f / side_size;
	size_t i = 0;
	for (float y = grid_size * 0.5f; y < 1; y += grid_size) {
		for (float x = grid_size * 0.5f + (rand() % 3 - 1) * grid_size * 0.25f; x < 1; x += grid_size) {
			for (float z = grid_size * 0.5f + (rand() % 3 - 1) * grid_size * 0.25f; z < 1; z += grid_size) {
				if (i >= nbParticles) break;
				waterParticles[i] = WaterParticle(Vec3(x-0.5f, y-0.5f, z-0.5f));
				i++;
			}
		}
	}

	// Définitions of boundaries
	plans = (Plan*) malloc(sizeof(Plan) * 6);
	// If the point is toward the normal, the point is in the boundarie
	plans[0] = Plan{ Vec3(-2, 0, 0), Vec3(1, 0, 0) };
	plans[1] = Plan{ Vec3(0, 2, 0), Vec3(0, -1, 0) };
	plans[2] = Plan{ Vec3(0, 0, -1), Vec3(0, 0, 1) };
	plans[3] = Plan{ Vec3(2, 0, 0), Vec3(-1, 0, 0) };
	plans[4] = Plan{ Vec3(0, -2, 0), Vec3(0, 1, 0) };
	plans[5] = Plan{ Vec3(0, 0, 1), Vec3(0, 0, -1) };
	nb_plans = 6;

	particleGrid = ParticleGrid(DENSITY_RADIUS, Vec3(-2, -2, -1), Vec3(2, 2, 1));
	particleGrid.setParticles(waterParticles, nbParticles);

	/*
	particleGrid.startIterationNears(Vec3(0, 0, 0));
	WaterParticle* particle = particleGrid.next();
	while (particle != NULL) {
		std::cout << particle->getPos();
		particle = particleGrid.next();
	}
	*/

	volume_inv = 1.0f / (4 * 3.141592 * (2 - std::exp(-DENSITY_RADIUS) * (2 + 2 * DENSITY_RADIUS + DENSITY_RADIUS * DENSITY_RADIUS + DENSITY_RADIUS * DENSITY_RADIUS * DENSITY_RADIUS * 0.333)));
}


void WaterSimulator::update() {
	computeCollisions();
	particleGrid.setParticles(waterParticles, nbParticles);

	for (int i = 0; i < nbParticles; i++) {
		waterParticles[i].setDensity(compute_density(waterParticles[i].getPos()));
		waterParticles[i].setPressureForce(Vec3(0, 0, 0));
		waterParticles[i].computePressure();
	}


	for (int i = 0; i < nbParticles; i++) {
		particleGrid.startIterationNears(waterParticles[i].getPos());
		WaterParticle* particle2 = particleGrid.next();
		while (particle2 != NULL) {
			// If the 2 pointers are different, if it's not twice the same particle
			//if (waterParticles + i != particle2) {
			Vec3 p1 = waterParticles[i].getPos();
			Vec3 p2 = particle2->getPos();
			// p1 < p2 in lexicographic order (to prevent doubles)
			if (p1.getZ() < p2.getZ() || (p1.getZ() == p2.getZ() && p1.getY() < p2.getY()) || (p1.getZ() == p2.getZ() && p1.getY() == p2.getY() && p1.getX() < p2.getX())) {
				Vec3 currentPressure1 = waterParticles[i].getPressureForce();
				Vec3 currentPressure2 = particle2->getPressureForce();
				Vec3 to_add = compute_pressure_force(waterParticles[i], *particle2);
				waterParticles[i].setPressureForce(currentPressure1 + to_add);
				particle2->setPressureForce(currentPressure2 - to_add);
			}
			particle2 = particleGrid.next();
		}
		/*
		for (WaterParticle* particle2 : particleGrid.getNear(waterParticles[i].getPos())) {
			// If the 2 pointers are different, if it's not twice the same particle
			if (waterParticles + i != particle2) {
				Vec3 currentPressure = waterParticles[i].getPressureForce();
				Vec3 to_add = compute_pressure_force(waterParticles[i], *particle2);
				waterParticles[i].setPressureForce(currentPressure + to_add);
			}
		}
		*/
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
	
	particleGrid.startIterationNears(pos);
	WaterParticle* particle = particleGrid.next();
	while (particle != NULL) {
		// Instead of using the position, I use the position + speed, it gives a more stable solution.
		Vec3 particlePos = particle->getPos();// + particle->getSpeed();
		float d = dist(particlePos, pos);
		res += smoothing_func(d) * particle->getMass();
		particle = particleGrid.next();
	}
	/*
	for (WaterParticle* particle: particleGrid.getNear(pos)) {
		// Instead of using the position, I use the position + speed, it gives a more stable solution.
		Vec3 particlePos = particle->getPos() + particle->getSpeed();
		float d = dist(particlePos, pos);
		res += smoothing_func(d) * particle->getMass();
	}
	*/
	/*
	for (int i = 0; i < nbParticles; i++) {
		Vec3 particlePos = waterParticles[i].getPos();
		float d = dist(particlePos, pos);
		res += smoothing_func(d) * waterParticles[i].getMass();
	}*/
	

	// It is the integral of the smoothing function on the sphere
	// It is equal to 4 * pi * (2 - exp(-r) * (2 + 2r + r^2 + r^3/3))
	return res * volume_inv;
}


Vec3 WaterSimulator::compute_pressure_force(WaterParticle particle1, WaterParticle particle2) {

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













ParticleGrid::ParticleGrid(float tile_size, Vec3 start, Vec3 end) {
	this->start = start;
	this->end = end;
	this->tile_size = tile_size;
	Vec3 size = end - start;
	width  = ceil(size.getX() / tile_size);
	height = ceil(size.getY() / tile_size);
	depth  = ceil(size.getZ() / tile_size);

	grid = new std::vector<WaterParticle*>[width * height * depth];

	current_particle_index = 0;
	current_tile_index = 0;
	current_tile_number = 0;
}

int ParticleGrid::vec2index(Vec3 vec) {
	// We offset the position of the particle with the start position of the box, and then we scale of 1/tile_size
	int x = std::floor((vec.getX() - start.getX()) / tile_size);
	int y = std::floor((vec.getY() - start.getY()) / tile_size);
	int z = std::floor((vec.getZ() - start.getZ()) / tile_size);
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (z < 0) z = 0;
	if (x >= width)  x = width  - 1;
	if (y >= height) y = height - 1;
	if (z >= depth)  z = depth  - 1;
	return x + y * width + z * width * height;
}


int ParticleGrid::vec2index_not_protected(Vec3 vec) {
	int x = std::floor((vec.getX() - start.getX()) / tile_size);
	int y = std::floor((vec.getY() - start.getY()) / tile_size);
	int z = std::floor((vec.getZ() - start.getZ()) / tile_size);
	if (x == width)  x = width - 1;
	if (y == height) y = height - 1;
	if (z == depth)  z = depth - 1;
	return x + y * width + z * width * height;
}



Vec3 ParticleGrid::index2vec(int index) {
	int z = index / (width * height);
	int r_z = index % (width * height);
	int y = r_z % width;
	int x = r_z / width;
	// We add 0.5 so that it is the position of the center of the tile
	return start + Vec3(x+0.5, y+0.5, z+0.5) * tile_size;
}


void ParticleGrid::setParticles(WaterParticle* particles, size_t nbParticles) {
	// Clear all the vectors
	for (int i = 0; i < width * height * depth; i++) {
		grid[i].clear();
	}

	//Put the particles in the grid according to their position
	for (int i = 0; i < nbParticles; i++) {
		int index = vec2index(particles[i].getPos());
		// particles + i is the pointer to i-th water particle
		grid[index].push_back(particles + i);
	}
}


std::vector<WaterParticle*> ParticleGrid::getNear(Vec3 point) {
	std::vector<WaterParticle*> res = std::vector<WaterParticle*>();
	int point_index = vec2index(point);

	for (int offset_z = -1; offset_z <= 1; offset_z++) {
		for (int offset_y = -1; offset_y <= 1; offset_y++) {
			for (int offset_x = -1; offset_x <= 1; offset_x++) {
				int index = point_index + offset_x + offset_y * width + offset_z * width * height;
				if (index >= 0 && index < width * height * depth) {
					std::copy(grid[index].begin(), grid[index].end(), std::back_inserter(res));
				}
			}
		}
	}
	return res;
}


void ParticleGrid::startIterationNears(Vec3 point) {
	// The tile offsetted by (-1, -1, -1)
	current_tile_index = vec2index(point) - 1 - width - width * height; 
	
	current_tile_number = 0;
	current_particle_index = 0;
}

WaterParticle* ParticleGrid::next() {
	// While the current tile is not good, we look at the next one
	while (current_tile_index < 0 || current_tile_index >= width * height * depth || current_particle_index >= grid[current_tile_index].size()) {
		next_tile();

		// Returns NULL if we checked each tile
		if (current_tile_number >= 27) {
			return NULL;
		}
	}

	WaterParticle* res = grid[current_tile_index][current_particle_index];
	current_particle_index++;
	return res;
}

void ParticleGrid::next_tile() {
	current_particle_index = 0;
	current_tile_number++;
	if (current_tile_number % 9 == 0) {
		// We increase the depth of the tile, and reset the horizontal and vertical position
		current_tile_index += width * height - 2 * width - 2;
	}
	else if (current_tile_number % 3 == 0) {
		// We increase the y-axis of the tile, and reset the x-axis
		current_tile_index += width - 2;
	} 
	else {
		// We increase the x-axis of the tile
		current_tile_index++;
	}
}