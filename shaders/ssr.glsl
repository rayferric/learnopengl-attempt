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

in vec2 vTexCoord;

layout(location=5)out vec3 oColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gMRA;
uniform sampler2D gEmission;
uniform sampler2D gColor;

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform float uCameraPosition;

#define MAX_BIN 16
#define MAX_STEPS 64
#define MIN_RAY_STEP 0.1
#define MAX_RAY_STEP 1.0
#define RAY_MUL 1.05
#define TOLERANCE 0.2

vec3 fresnelSchlickRoughness(float NdotV, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - NdotV, 5.0);
} 

vec2 getProjected(in vec3 position) {
	vec4 projected = uProjectionMatrix * vec4(position, 1.0);
	projected.xy /= projected.w;
    return projected.xy * 0.5 + 0.5;
}

void main() {
	vec3 position = (uViewMatrix * vec4(texture(gPosition, vTexCoord).xyz, 1.0)).xyz;
	vec3 normal = (uViewMatrix * vec4(texture(gNormal, vTexCoord).xyz, 0.0)).xyz;
	vec3 albedo = texture(gAlbedo, vTexCoord).xyz;
	vec3 MRA = texture(gMRA, vTexCoord).xyz;
	float metallic = MRA.x;
	float roughness = MRA.y;
	float ao = MRA.z;
	vec3 emission = texture(gEmission, vTexCoord).xyz;
	vec3 color = texture(gColor, vTexCoord).xyz;
	if(roughness >= 0.5 || metallic < 0.1) {
        oColor = color;
        return;
    }
	
	vec3 N = normalize(normal);
	vec3 V = normalize(uCameraPosition - position);
	vec3 R = normalize(reflect(normalize(position), N));
	
	float NdotV = max(0.0, dot(N, V));
	
	bool found = false;
	float step = clamp(position.z*position.z*0.01, MIN_RAY_STEP, MAX_RAY_STEP);
	vec2 projected;
    for(int i=0; i<MAX_STEPS; i++) {
        position += R * step;
 
        projected = getProjected(position);
		if(projected.x < 0.0 || projected.x > 1.0 || projected.y < 0.0 || projected.y > 1.0 || position.z > 0.0)break;
		float compare = (uViewMatrix * vec4(texture(gPosition, projected).xyz, 1.0)).z;
 
        if(position.z < compare) {
			for(int j=0; j<MAX_BIN; j++) {
				position -= R * (step *= 0.5);
				projected = getProjected(position);
				compare = (uViewMatrix * vec4(texture(gPosition, projected).xyz, 1.0)).z;
				if(position.z > compare)position += R * step;
				else if(compare - position.z < TOLERANCE) {
					found = true;
					break;
				}
			}
			break;
		}
		step *= RAY_MUL;
    }
	vec3 F0 = mix(vec3(0.04), albedo, metallic);
	vec3 F = fresnelSchlickRoughness(NdotV, F0, roughness);
	
	vec3 kD = vec3(1.0) - F;
	kD *= vec3(1.0) - metallic;
	vec3 specular = texture(gColor, projected).xyz * F;
	vec3 reflection = specular;
	reflection *= ao;
	reflection += emission;
	
	float fade = clamp(1.0 - pow(max(abs(projected.x-0.5), abs(projected.y-0.5))*2.0, 16.0), 0.0, 1.0);
	//if(found)oColor = texture(gColor, projected).xyz * fade;
	//else oColor = vec3(1.0);
	oColor = mix(color, reflection, float(found)*fade);
}
