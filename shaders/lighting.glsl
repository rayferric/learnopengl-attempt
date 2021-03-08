@vertex
#version 330 core

layout(location=0)in vec3 aVertex;
layout(location=1)in vec2 aTexCoord;

out vec2 vTexCoord;

void main() {
	vTexCoord = aTexCoord;
	
	gl_Position = vec4(aVertex, 1.0);
}



@fragment
#version 330 core
#define PI 3.14159265359
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

in vec2 vTexCoord;

layout(location=5)out vec3 oColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gMRA;
uniform sampler2D gEmission;

struct Light {
	int type;
	vec3 color;
	vec3 ambient;
	vec3 direction;
	vec3 position;
	float range;
	float angle;
};
uniform Light uLight;

uniform vec3 uCameraPosition;



float distributionGGX(in float NdotH, in float roughness) {
    float roughness2 = roughness * roughness;
	float roughness4 = roughness2 * roughness2;
    float denom = NdotH * NdotH * (roughness4 - 1.0) + 1.0;
    return roughness4 / (PI * denom * denom);
}

float geometrySchlickGGX(in float NdotV, in float roughness) {
    return NdotV / (NdotV * (1.0 - roughness) + roughness);
}

float geometrySmith(in float NdotV, in float NdotL, in float roughness) {
    return geometrySchlickGGX(NdotV, roughness) * geometrySchlickGGX(NdotL, roughness);
}

vec3 fresnelSchlick(in float VdotH, in vec3 F0) {
	return (F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0));
}

float computeAttenuation(in vec3 L) {
	if(uLight.type == DIRECTIONAL_LIGHT)return 1.0;
	float distance = length(L);
	float attenuation = clamp(1.0 - (distance * distance) / (uLight.range * uLight.range), 0.0, 1.0);
	return attenuation * attenuation;
}

vec3 getLightVector(in vec3 position) {
	if(uLight.type == DIRECTIONAL_LIGHT)return -uLight.direction;
	return uLight.position - position;
}

void main() {
	vec3 position = texture(gPosition, vTexCoord).xyz;
	vec3 normal = texture(gNormal, vTexCoord).xyz;
	vec3 albedo = texture(gAlbedo, vTexCoord).xyz;
	vec3 MRA = texture(gMRA, vTexCoord).xyz;
	float metallic = MRA.x;
	float roughness = max(0.01, MRA.y);
	float ao = MRA.z;
	vec3 emission = texture(gEmission, vTexCoord).xyz;
	
	vec3 color;
	{
		vec3 N = normalize(normal);
		vec3 L = getLightVector(position);
		vec3 V = normalize(uCameraPosition - position);
		vec3 H = normalize(L + V);
		vec3 R = reflect(-V, N);
		
		float NdotL = max(0.0, dot(N, L));
		float NdotV = max(0.0, dot(N, V));
		float NdotH = max(0.0, dot(N, H));
		float VdotH = max(0.0, dot(V, H));
		
		vec3 F0 = mix(vec3(0.04), albedo, metallic);
		float D = distributionGGX(NdotH, roughness);
		float G = geometrySmith(NdotV, NdotL, roughness);
		vec3 F = fresnelSchlick(VdotH, F0);
		
		vec3 kD = vec3(1.0) - F;
		kD *= 1.0 - metallic;
		
		vec3 specular = (D * G * F) / max(0.001, 4.0 * NdotL * NdotV); 
		vec3 diffuse = kD * albedo / PI;
		
		vec3 radiance = uLight.color * computeAttenuation(L);
		
		color = (diffuse + specular) * radiance * NdotL;
		color += uLight.ambient * albedo;
		color *= ao;
	}
	oColor = color;
}
