#pragma once

#include "Shader.h"


class ComputeShader : public Shader {
private:
	GLuint program;
	bool isActivated;

	GLuint VAO;

public:
	ComputeShader(std::string computeShader);
	ComputeShader();

	void activate();

	void init(struct VertexAttributes* vertices, int number_vertices);
};
