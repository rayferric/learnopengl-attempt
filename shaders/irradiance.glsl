@vertex
#version 330 core

layout(location=0)in vec3 aVertex;

out vec3 vFragPos;

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

void main() {
	vFragPos = aVertex;
	
	gl_Position = uProjectionMatrix * uViewMatrix * vec4(vFragPos, 1.0);
}



@fragment
#version 330 core
#define PI 3.14159265359
#define SAMPLE_COUNT 512u

in vec3 vFragPos;

layout(location=0)out vec3 oColor;

uniform samplerCube uCubeMap;
uniform float uRoughness;
uniform int uResolution;

float distributionGGX(float NdotH, float roughness) {
    float roughness2 = roughness * roughness;
    float denom = NdotH * NdotH * (roughness2 - 1.0) + 1.0;
    return roughness2 / (PI * denom * denom);
}

vec3 importanceSampleGGX(vec2 Xi, vec3 N, float roughness) {
	float roughness2 = roughness * roughness;

	float phi = 2.0 * PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (roughness2 * roughness2 - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

	vec3 H;
	H.x = sinTheta * cos(phi);
	H.y = sinTheta * sin(phi);
	H.z = cosTheta;

	vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);

	return tangent * H.x + bitangent * H.y + N * H.z;
}

float radicalInverse_VdC(uint bits) {
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 hammersley(uint i, uint N) {
	return vec2(float(i) / float(N), radicalInverse_VdC(i));
}

void main() {
	vec3 N = normalize(vFragPos);

	vec4 irradiance = vec4(0.0);

	for(uint sample=0u; sample<SAMPLE_COUNT; sample++) {
		vec2 xi = hammersley(sample, SAMPLE_COUNT);
		vec3 H = importanceSampleGGX(xi, N, uRoughness);
		vec3 L = normalize(2.0 * dot(N, H) * H - N);
		
		float NdotL = max(0.0, dot(N, L));
		float NdotH = max(0.0, dot(N, H));
		
		//Gather mipmap level
		float D   = distributionGGX(NdotH, uRoughness);
		float pdf = (D * NdotH / (4.0 * NdotH)) + 0.0001; 
		float saTexel  = 4.0 * PI / (6.0 * uResolution * uResolution);
		float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);
		float mipmapLevel = uRoughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel); 
		//
		
		if(NdotL > 0.0) {
			irradiance.xyz += textureLod(uCubeMap, L, mipmapLevel).xyz * NdotL;
			irradiance.w += NdotL;
		}
	}
	irradiance.xyz /= irradiance.w;

	oColor = irradiance.xyz;
}