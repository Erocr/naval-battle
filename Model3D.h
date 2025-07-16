#pragma once

#include <vector>
#include "Mesh.h"
#include "Vec3.h"
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Model3D {
private:
	std::vector<Mesh*> meshes;
	glm::mat4 transform;

public:
	Model3D(std::vector<Mesh*> meshes_);

	void translate(Vec3 v);
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);
	void rotate(Vec3 angles);

	void draw(Shader shader) const;
};