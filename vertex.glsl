#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 color;

out vec4 vertexColor;

void main()
{
    vertexColor = color;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    //gl_Position.xyz /= gl_Position.w;
}
