#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 atexCoord;

out vec2 pos;

void main()
{   
    pos = (aPos.xy + vec2(1, 1)) * 0.5;
    gl_Position = vec4(aPos, 1.0);
}