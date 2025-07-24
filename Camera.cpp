#include "Camera.h"


Camera::Camera() {
	transform = glm::mat4(1.0);
}

void Camera::update(Shader shader) {
	glm::mat4 inverse = glm::inverse(transform);
	shader.putUniform("view", transform);
	shader.putUniform("view_inv", inverse);
}

void Camera::translate(Vec3 v) {
	glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(v.getX(), v.getY(), v.getZ()));
	transform = transform * translate;
}

void Camera::rotateX(float angle) {
	transform = glm::rotate(transform, glm::radians(angle), glm::vec3(1.0, 0.0, 0.0));
}

void Camera::rotateY(float angle) {
	transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
}

void Camera::rotateZ(float angle) {
	transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
}

void Camera::rotate(Vec3 angles) {
	rotateX(angles.getX());
	rotateY(angles.getY());
	rotateZ(angles.getZ());
}