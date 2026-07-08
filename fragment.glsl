#version 330 core

uniform vec3 camPos;
uniform vec4 ambient;
uniform int nbLights;
uniform vec3 lightPos[10];
uniform vec4 lightColor[10];

uniform bool affectedByLight;

out vec4 fragColor;

in vec3 normal_frag;
in vec3 pos;
in vec2 texCoord;

uniform sampler2D theTexture;


void main() {
    vec4 color = texture(theTexture, texCoord);
    if (affectedByLight) {
        vec3 pixelDir = pos - camPos;
        vec3 normal = normal_frag;

        if (dot(normal_frag, pixelDir) > 0) {
            normal = -normal;
        }
        vec3 view_dir = normalize(camPos - pos);
        vec3 normalized_normal = normalize(normal);
        fragColor = ambient * color;
        for (int i=0; i < nbLights && i < 10; i++) {
            vec3 light_dir = normalize(lightPos[i]-pos);
            float diff_strength = dot(normalized_normal, light_dir);
            vec4 lColor = lightColor[i];
            float dist = distance(pos, lightPos[i]);
            if (dist < 1) 
                dist = 1;
            if (diff_strength < 0) {
                diff_strength = 0;
            }
            fragColor += diff_strength / dist * lColor * color;

            vec3 H = normalize(view_dir + light_dir);
            float spec_strength = dot(H, normalized_normal);
            spec_strength = pow(spec_strength, 128);
            fragColor += spec_strength / dist * lColor * color * 0.3;
        }
    } else {
        fragColor = color;
    }
}