#include "Mesh.h";


Mesh::Mesh(std::vector<VertexAttributes> vertices, size_t start, VertexAttributes* buffer, GLuint texture) {
	memcpy(buffer + start, vertices.data(), vertices.size() * sizeof(VertexAttributes));
	this->start = start;
	end = start + vertices.size();
	this->texture = texture;
}

size_t Mesh::getStart() const {
	return start;
}

size_t Mesh::getEnd() const {
	return end;
}

void Mesh::draw() const {
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, start, end-start);
}
