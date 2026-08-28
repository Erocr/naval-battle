#version 330 core

out vec4 fragColor;

in vec2 pos;

uniform mat4 projectionInverse;
uniform mat4 viewTransform;
uniform vec3 camPos;
uniform sampler2D previousColor;
uniform sampler2D depthBuffer;
uniform float near;
uniform float far;

uniform vec4 ambient;
uniform int nbLights;
uniform vec3 lightPos[10];
uniform vec4 lightColor[10];

uniform int nbWaterParticles;
uniform vec3 waterParticles[1000];


float sdSphere(vec3 p, vec3 sphereCenter, float radius) {
	return distance(sphereCenter, p) - radius;
}

float smin(float a, float b, float k) {
  float h = clamp(0.5 + 0.5*(a-b)/k, 0.0, 1.0);
  return mix(a, b, h) - k*h*(1.0-h);
}

float sdf(vec3 p) {
	float res = 100000;
	for (int i=0; i<1000 && i<nbWaterParticles; i++) {
		vec3 offset = vec3(1, 0, -3);
		res = smin(res, sdSphere(p, waterParticles[i] + offset, 0.1), 0.25);
	}
	return res;
}

vec3 gradient(vec3 p, float h) {
	vec3 p_x = p + vec3(h, 0, 0);
	vec3 p_y = p + vec3(0, h, 0);
	vec3 p_z = p + vec3(0, 0, h);
	float s = sdf(p);
	return vec3((sdf(p_x) - s)/h,
				(sdf(p_y) - s)/h,
				(sdf(p_z) - s)/h);
}

vec3 rayMarch(vec3 pos, vec3 dir, float depth, out float dist, out int nb_it) {
	dist = 0;
	nb_it = 0;
	while (dist < depth ) {
		nb_it ++;
		float new_dist = sdf(pos);
		if (new_dist <= 0.00001) {
			return pos;
		}
		pos += dir * new_dist;
		dist += new_dist;
	}
	dist = -1;
	return vec3(0, 0, 0);
}

void main() {
	vec4 nearCameraPos = vec4((pos - 0.5) * 2, -1, 1);
	vec4 direction_4d = projectionInverse * nearCameraPos;
	vec3 dir = normalize(direction_4d.xyz);
	dir = mat3(viewTransform) * dir;

	int nb_it;
	float dist;
	float depth = 1 / (texture(depthBuffer, pos).r * (1/far - 1/near) + 1/near);
	vec3 point = rayMarch(camPos, dir, depth, dist, nb_it);
	
	if (dist < 0){
		float clamped_dist = texture(depthBuffer, pos).r;
		fragColor = texture(previousColor, pos);
		return;
	}

	// Compute the color as in the depth buffer
	float clamped_dist = (1/min(depth, dist) - 1/near) / (1/far - 1/near);

	// compute the normal
	vec3 normal = normalize(gradient(point, 0.0001));

	// Compute the result color
	vec4 color = vec4(1);
	vec3 view_dir = normalize(camPos - point);

	fragColor = ambient * color;  // ambient part

	for (int i=0; i < nbLights && i < 10; i++) {
		float lightDist = distance(lightPos[i], point) * 0.2;
        vec3 light_dir = normalize(lightPos[i]-point);
        float diff_strength = dot(normal, light_dir);  // strength of the diffuse part
        vec4 lColor = lightColor[i];
        if (lightDist < 1) 
            lightDist = 1;
        if (diff_strength < 0) {
            diff_strength = 0;
        }

        fragColor += diff_strength / lightDist * lColor * color;  // Adding the diffuse part

        vec3 H = normalize(view_dir + light_dir);
        float spec_strength = dot(H, normal);  // strength of the specular part
        spec_strength = pow(spec_strength, 64);
        fragColor += spec_strength / lightDist * lColor * color * 0.3;  // Adding the specular part
    }

}