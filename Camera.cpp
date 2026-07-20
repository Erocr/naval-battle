#include "Camera.h"
#include <iostream>


Camera::Camera() {
	transform = glm::mat4(1.0);
	rotY = 0;
}

void Camera::update(Shader shader, Shader shader2) {
	glm::mat4 inverse = glm::inverse(transform);
	Vec3 camPos = getPos();
	shader.activate();
	shader.putUniform("view", transform);
	shader.putUniform("view_inv", inverse);
	shader.putUniform("camPos", camPos.getX(), camPos.getY(), camPos.getZ());

	shader2.activate();
	shader2.putUniform("camPos", camPos.getX(), camPos.getY(), camPos.getZ());
	shader2.putUniform("rotY", rotY);
	shader2.putUniform("viewTransform", transform);
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
	rotY += glm::radians(angle);
}

void Camera::rotateZ(float angle) {
	transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
}

void Camera::rotate(Vec3 angles) {
	rotateX(angles.getX());
	rotateY(angles.getY());
	rotateZ(angles.getZ());
}

Vec3 Camera::getPos() {
	return Vec3(transform[3].x, transform[3].y, transform[3].z);
}