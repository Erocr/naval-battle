#version 330 core

uniform vec4 ambient;
uniform int nbLights;
uniform vec3 lightPos[10];
uniform vec4 lightColor[10];

uniform bool affectedByLight;

out vec4 fragColor;

in vec3 normal_frag;
in vec3 pos;
in vec2 texCoord;
in vec3 playerPos;

uniform sampler2D theTexture;


void main() {
    vec4 color = texture(theTexture, texCoord);
    if (affectedByLight) {
        vec3 pixelDir = pos - playerPos;
        vec3 normal = normal_frag;
        if (dot(normal_frag, pixelDir) > 0) {
            normal = -normal;
        }
        vec3 normalized_normal = normalize(normal);
        fragColor = ambient * color;
        for (int i=0; i < nbLights && i < 10; i++) {
            float strength = dot(normalized_normal, normalize(lightPos[i]-pos));
            if (strength < 0) {
                strength = 0;
            }
            float dist = distance(pos, lightPos[i]) * 2;
            if (dist < 1) dist = 1;
            fragColor += strength / dist * lightColor[i] * color;
        }
    } else {
        fragColor = color;
    }
}