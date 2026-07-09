#include "Shader.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::string Shader::read_file(std::string file_name) {
	std::ifstream file;
	file.open(file_name);
	std::string line;
	std::string res = "";
	while (std::getline(file, line)) {
		res += line + '\n';
	}
	file.close();
	return res;
}

GLuint Shader::importShader(std::string file_name, int type) {
	std::string shader_source_s = read_file(file_name);
	const GLchar* shader_source = static_cast<const GLchar*>(shader_source_s.c_str());
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shader_source, nullptr);
	glCompileShader(shader);

	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		GLchar infoLog[100];
		glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog);
		printf("\nerreur de compilation dans le fragment shader (%i char):\n", maxLength);

		std::cerr << infoLog;

		glDeleteShader(shader);

		exit(-1);
	}

	return shader;
}


GLuint Shader::load_image(std::string file_name) {
	GLuint res;
	glGenTextures(1, &res);
	glBindTexture(GL_TEXTURE_2D, res);
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file_name.data(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		int format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load " << file_name << std::endl;
		exit(-2);
	}
	stbi_image_free(data);
	return res;
}



void Shader::activate() {
	glUseProgram(program);
	isActivated = true;
}

void Shader::putUniform(std::string variable_name, int v) {
	int location = glGetUniformLocation(program, variable_name.c_str());
	glUniform1i(location, v);
}

void Shader::putUniform(std::string variable_name, float v) {
	int location = glGetUniformLocation(program, variable_name.c_str());
	glUniform1f(location, v);
}

void Shader::putUniform(std::string variable_name, float v1, float v2) {
	int location = glGetUniformLocation(program, variable_name.c_str());
	glUniform2f(location, v1, v2);
}

void Shader::putUniform(std::string variable_name, float v1, float v2, float v3) {
	int location = glGetUniformLocation(program, variable_name.c_str());
	glUniform3f(location, v1, v2, v3);
}

void Shader::putUniform(std::string variable_name, float v1, float v2, float v3, float v4) {
	int location = glGetUniformLocation(program, variable_name.c_str());
	glUniform4f(location, v1, v2, v3, v4);
}

void Shader::putUniform(std::string variable_name, std::vector<float> v) {
	int location = glGetUniformLocation(program, variable_name.c_str());
	glUniform1fv(location, v.size(), v.data());
}

void Shader::putUniform(std::string variable_name, std::vector<Vec2> v) {
	int location = glGetUniformLocation(program, variable_name.c_str());
	glUniform2fv(location, v.size(), (float*) v.data());
}

void Shader::putUniform(std::string variable_name, std::vector<Vec3> v) {
	int location = glGetUniformLocation(program, variable_name.c_str());
	glUniform3fv(location, v.size(), (float*) v.data());
}

void Shader::putUniform(std::string variable_name, std::vector<Vec4> v) {
	int location = glGetUniformLocation(program, variable_name.c_str());
	glUniform4fv(location, v.size(), (float*) v.data());
}

void Shader::putUniform(std::string variable_name, glm::mat4 v) {
	int location = glGetUniformLocation(program, variable_name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(v));
}

void Shader::putUniformT(std::string variable_name, glm::mat4 v) {
	int location = glGetUniformLocation(program, variable_name.c_str());
	glUniformMatrix4fv(location, 1, GL_TRUE, glm::value_ptr(v));
}


int Shader::sizeofGL(GLenum type) {
	switch (type) {
		case GL_FLOAT: return sizeof(float);
		case GL_INT: return sizeof(int);
		case GL_BOOL: return sizeof(bool);
		default: std::cerr << "Type non reconnu.";
	}
}
