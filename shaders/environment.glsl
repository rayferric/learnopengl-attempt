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
#define MAX_IRRADIANCE_LOD 4

in vec2 vTexCoord;

layout(location=0)out vec3 oPosition;
layout(location=1)out vec3 oNormal;
layout(location=2)out vec3 oAlbedo;
layout(location=3)out vec3 oMRA;
layout(location=4)out vec3 oEmission;
layout(location=5)out vec3 oColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gMRA;
uniform sampler2D gEmission;

uniform samplerCube uIrradianceMap;
uniform sampler2D uBrdfLUT;

uniform vec3 uCameraPosition;



vec3 fresnelSchlickRoughness(float NdotV, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - NdotV, 5.0);
} 

void main() {
	vec3 position = texture(gPosition, vTexCoord).xyz;
	vec3 normal = texture(gNormal, vTexCoord).xyz;
	vec3 albedo = texture(gAlbedo, vTexCoord).xyz;
	vec3 MRA = texture(gMRA, vTexCoord).xyz;
	float metallic = MRA.x;
	float roughness = MRA.y;
	float ao = MRA.z;
	vec3 emission = texture(gEmission, vTexCoord).xyz;
	
	vec3 color;
	{
		vec3 N = normalize(normal);
		vec3 V = normalize(uCameraPosition - position);
		vec3 R = reflect(-V, N);
		
		float NdotV = max(0.0, dot(N, V));
		
		vec3 F0 = mix(vec3(0.04), albedo, metallic);
		vec3 F = fresnelSchlickRoughness(NdotV, F0, roughness);
		
		vec3 diffuseIrradiance = pow(textureLod(uIrradianceMap, N, MAX_IRRADIANCE_LOD).xyz, vec3(2.2));
		vec3 specularReflection = pow(textureLod(uIrradianceMap, R, roughness * MAX_IRRADIANCE_LOD).xyz, vec3(2.2));
		
		vec3 kD = vec3(1.0) - F;
		kD *= vec3(1.0) - metallic;
		vec3 diffuse = kD * diffuseIrradiance * albedo;
		vec2 brdf = texture(uBrdfLUT, vec2(NdotV, roughness)).xy;
		vec3 specular = specularReflection * (F * brdf.x + brdf.y);
		
		color = diffuse + specular;
		color *= ao;
		color += emission;
	}
	oPosition = position;
	oNormal = normal;
	oAlbedo = albedo;
	oMRA = vec3(metallic, roughness, ao);
	oEmission = emission;
	oColor = color;
}
