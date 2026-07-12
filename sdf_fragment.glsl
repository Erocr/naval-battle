#version 330 core

out vec4 fragColor;

in vec2 pos;

uniform sampler2D previousColor;
uniform sampler2D depthBuffer;

void main() {
	fragColor = texture(previousColor, pos);
}