#pragma once

#include "VertexAttributes.h"

#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <string>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader {
private:
	GLuint program;
	bool isActivated;

	GLuint VAO;

public:
	Shader(std::string vert_file_name, std::string frag_file_name);

	void activate();

	void putUniform(std::string variable_name, int v);
	void putUniform(std::string variable_name, float v);
	void putUniform(std::string variable_name, float v1, float v2);
	void putUniform(std::string variable_name, float v1, float v2, float v3);
	void putUniform(std::string variable_name, float v1, float v2, float v3, float v4);
	void putUniform(std::string variable_name, glm::mat4 v);
	void putUniformT(std::string variable_name, glm::mat4 v);

	void init(struct VertexAttributes* vertices, int number_vertices);

	void test_in_loop();
};
