#version 330 core

uniform vec4 ambient;
uniform int nbLights;
uniform vec3 lightPos[10];
uniform vec4 lightColor[10];

out vec4 fragColor;

in vec4 vertexColor;
in vec3 normal_frag;
in vec3 pos;

void main() {
    vec3 normalized_normal = normalize(normal_frag);
    fragColor = ambient * vertexColor;
    for (int i=0; i < nbLights && i < 10; i++) {
        float strength = abs(dot(normalized_normal, normalize(lightPos[i]-pos)));
        float dist = distance(pos, lightPos[i]) * 2;
        if (dist < 1) dist = 1;
        fragColor += strength / dist * lightColor[i] * vertexColor;
        if (strength / dist >= 0.8) fragColor = vec4(1, 1, 1, 1); 
    }
}