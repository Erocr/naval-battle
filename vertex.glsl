#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;

out vec4 vertexColor;
out vec3 normal_frag;
out vec3 pos;

void main()
{
    pos = (model * vec4(aPos, 1.0)).xyz;

    gl_Position = projection * view * vec4(pos, 1.0);
    //gl_Position.xyz /= gl_Position.w;

    normal_frag = (model * vec4(normal, 0.0)).xyz;
    vertexColor = color;

}
