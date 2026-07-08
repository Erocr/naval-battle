#version 330 core

uniform mat4 model;
uniform mat4 model_inv_t;
uniform mat4 view;
uniform mat4 view_inv;
uniform mat4 projection;

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 atexCoord;

out vec3 normal_frag;
out vec3 pos;
out vec2 texCoord;

void main()
{
    pos = (model * vec4(aPos, 1.0)).xyz;

    gl_Position = projection * view_inv * vec4(pos, 1.0);

    normal_frag = (model_inv_t * vec4(normal, 0.0)).xyz;
    texCoord = atexCoord;
}
