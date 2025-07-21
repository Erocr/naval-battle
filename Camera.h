#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Camera {
private:
	glm::mat4 transform;

public:
	Camera();
	
	void translate(Vec3);
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);
	void rotate(Vec3);

	void update(Shader shader);
};