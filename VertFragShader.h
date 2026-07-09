#pragma once

#include "VertexAttributes.h"

#include "Shader.h"


class VertFragShader : public Shader {
private:
	GLuint program;
	bool isActivated;

	GLuint VAO;

public:
	VertFragShader(std::string vert_file_name, std::string frag_file_name);
	VertFragShader();

	void init(struct VertexAttributes* vertices, int number_vertices);
};
