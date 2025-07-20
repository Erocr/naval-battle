#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 ambient;
uniform int nbLights;
uniform vec3 lightPos[10];
uniform vec4 lightColor[10];

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;

out vec4 vertexColor;

void main()
{
    vec3 vertexPos = (model * vec4(aPos, 1.0)).xyz;
    gl_Position = projection * view * vec4(vertexPos, 1.0);
    //gl_Position.xyz /= gl_Position.w;

    vec3 normalized_normal = normalize(normal);
    vertexColor = ambient * color;
    for (int i=0; i < nbLights && i < 10; i++) {
        float strength = abs(dot(normalized_normal, normalize(lightPos[i]-vertexPos)));
        float dist = distance(vertexPos, lightPos[i]);
        if (dist < 1) dist = 1;
        vertexColor += strength / dist * lightColor[i] * color;
        if (lightColor[i].x == 0) vertexColor = vec4(1, 1, 1, 1); 
    }

}
