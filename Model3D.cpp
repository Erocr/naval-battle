#include "Model3D.h"

Model3D::Model3D(std::vector<Mesh*> meshes_) {
	meshes = meshes_;
	transform = glm::mat4(1.0);
}

void Model3D::translate(Vec3 v) {
	glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(v.getX(), v.getY(), v.getZ()));
	transform = transform * translate;
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

void Model3D::scale(Vec3 values) {
	transform = glm::scale(transform, glm::vec3(values.getX(), values.getY(), values.getZ()));
}

void Model3D::draw(Shader shader) const {
	shader.putUniform("model", transform);
	shader.putUniform("model_inv_t", glm::transpose(glm::inverse(transform)));
	for (const Mesh *mesh: meshes) {
		(*mesh).draw();
	}
}