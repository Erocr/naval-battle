#include "VertFragShader.h"
#include <iostream>


VertFragShader::VertFragShader(std::string vert_file_name, std::string frag_file_name) {
	program = glCreateProgram();

	printf("compiling shaders ...  ");

	GLuint vert_shader = importShader(vert_file_name, GL_VERTEX_SHADER);
	GLuint frag_shader = importShader(frag_file_name, GL_FRAGMENT_SHADER);

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


VertFragShader::VertFragShader() { }



void VertFragShader::init(struct VertexAttributes* vertices, int number_vertices) {
	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAttributes) * number_vertices, vertices, GL_STATIC_DRAW);

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