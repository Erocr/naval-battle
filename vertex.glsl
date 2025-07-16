#version 330 core

uniform vec4 color;
uniform mat4 model;

layout(location = 0) in vec3 aPos;

out vec4 vertexColor;

void main()
{
    vertexColor = color;
    gl_Position = model * vec4(aPos, 1.0);
}
