#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 view_inv;
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

    gl_Position = projection * view_inv * vec4(pos, 1.0);
    //gl_Position.xyz /= gl_Position.w;

    vertexColor = color;
    normal_frag = (model * vec4(normal, 0.0)).xyz;
    if (dot(normal_frag, (view_inv * vec4(pos, 1)).xyz) > 0) {
        normal_frag = -normal_frag;
    }

}
