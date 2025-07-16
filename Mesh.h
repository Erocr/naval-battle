#pragma once

#include "VertexAttributes.h"
#include "Shader.h"

#include <string>

class Mesh {
private:
	size_t start;
	size_t end;

public:
	Mesh(std::string fileName, size_t start, VertexAttributes* buffer);
	Mesh(std::vector<VertexAttributes> vertices, size_t start, VertexAttributes* buffer);

	void draw() const;
	size_t getStart() const;
	size_t getEnd() const;
};
