#pragma once

#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

#include <GL/glew.h>




struct VertexAttributes {
	float aPos[3];
	float color[4];
	float normal[3];
};


const GLenum TYPES_VERTEX_ATTR[] = { GL_FLOAT, GL_FLOAT, GL_FLOAT };
const int SIZES_VERTEX_ATTR[] = { 3, 4, 3 };