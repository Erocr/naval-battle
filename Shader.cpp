#include "Shader.h"
#include <iostream>

std::string read_file(std::string file_name) {
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

Shader::Shader() {}


Shader::Shader(std::string vert_file_name, std::string frag_file_name) {

	printf("compiling shaders ...  ");
	std::string vert_source_s = read_file(vert_file_name);
	std::string frag_source_s = read_file(frag_file_name);

	const GLchar* vert_source = static_cast<const GLchar*>(vert_source_s.c_str());
	const GLchar* frag_source = static_cast<const GLchar*>(frag_source_s.c_str());

	GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vert_shader, 1, &vert_source, nullptr);
	glShaderSource(frag_shader, 1, &frag_source, nullptr);

	glCompileShader(vert_shader);
	glCompileShader(frag_shader);

	GLint isCompiled = 0;
	glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(frag_shader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(frag_shader, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(frag_shader);

		exit(-1);
	}
	isCompiled = 0;
	glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &maxLength);

		GLchar infoLog[100];
		glGetShaderInfoLog(vert_shader, maxLength, &maxLength, &infoLog[0]);
		printf("\nerreur de compilation dans le vertex shader (%i char):\n", maxLength);
		
		std::cerr << infoLog;

		glDeleteShader(vert_shader);
		glDeleteShader(frag_shader);

		exit(-1);
	}

	program = glCreateProgram();

	glAttachShader(program, vert_shader);
	glAttachShader(program, frag_shader);

	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(program);
		glDeleteShader(vert_shader);
		glDeleteShader(frag_shader);

		printf("\nerreur dans l'édition des liens (%i char):\n", maxLength);
		std::cerr << infoLog.data();
		exit(-1);
	}

	glDetachShader(program, vert_shader);
	glDetachShader(program, frag_shader);

	isActivated = false;

	printf("finished \n");
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
	glUniform1fv(location, v.size(), (float*) v.data());
}

void Shader::putUniform(std::string variable_name, std::vector<Vec3> v) {
	int location = glGetUniformLocation(program, variable_name.c_str());
	glUniform1fv(location, v.size(), (float*) v.data());
}

void Shader::putUniform(std::string variable_name, std::vector<Vec4> v) {
	int location = glGetUniformLocation(program, variable_name.c_str());
	glUniform1fv(location, v.size(), (float*) v.data());
}

void Shader::putUniform(std::string variable_name, glm::mat4 v) {
	int location = glGetUniformLocation(program, variable_name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(v));
}

void Shader::putUniformT(std::string variable_name, glm::mat4 v) {
	int location = glGetUniformLocation(program, variable_name.c_str());
	glUniformMatrix4fv(location, 1, GL_TRUE, glm::value_ptr(v));
}

int sizeofGL(GLenum type) {
	switch (type) {
	case GL_FLOAT: return sizeof(float);
	case GL_INT: return sizeof(int);
	case GL_BOOL: return sizeof(bool);
	default: std::cerr << "Type non reconnu.";
	}
}



void Shader::init(struct VertexAttributes* vertices, int number_vertices) {
	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAttributes)*number_vertices, vertices, GL_STATIC_DRAW);

	size_t length = sizeof(SIZES_VERTEX_ATTR) / sizeof(int);
	size_t tot_size = 0;
	for (size_t i = 0; i < length; i++) { tot_size += SIZES_VERTEX_ATTR[i] * sizeofGL(TYPES_VERTEX_ATTR[i]); }
	size_t offset = 0;
	for (size_t i = 0; i < length; i++) {
		int size = SIZES_VERTEX_ATTR[i] * sizeofGL(TYPES_VERTEX_ATTR[i]);
		glVertexAttribPointer(i, SIZES_VERTEX_ATTR[i], TYPES_VERTEX_ATTR[i], GL_FALSE, tot_size, (void*)offset);
		glEnableVertexAttribArray(i);
		offset += size;
	}

	activate();
}



void Shader::test_in_loop() {
	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
}
