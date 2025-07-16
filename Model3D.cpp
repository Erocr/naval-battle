#include "Model3D.h"

Model3D::Model3D(std::vector<Mesh*> meshes_) {
	meshes = meshes_;
	transform = glm::mat4(1.0);
}



void Model3D::translate(Vec3 v) {
	transform = glm::translate(transform, glm::vec3(v.getX(), v.getY(), v.getZ()));
}

void Model3D::rotateX(float angle) {
	transform = glm::rotate(transform, glm::radians(angle), glm::vec3(1.0, 0.0, 0.0));
}

void Model3D::rotateY(float angle) {
	transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
}

void Model3D::rotateZ(float angle) {
	transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
}

void Model3D::rotate(Vec3 angles) {
	rotateX(angles.getX());
	rotateY(angles.getY());
	rotateZ(angles.getZ());
}


void Model3D::draw(Shader shader) const {
	shader.putUniform("model", transform);
	for (const Mesh *mesh: meshes) {
		(*mesh).draw();
	}
}