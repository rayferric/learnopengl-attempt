@vertex
#version 330 core

layout(location=0)in vec3 aVertex;
layout(location=1)in vec2 aTexCoord;
layout(location=2)in vec3 aNormal;
layout(location=3)in vec3 aTangent;

out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vFragPos;
out mat3 vTBN;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

void main() {
	vTexCoord = aTexCoord;
	vNormal = normalize(mat3(transpose(inverse(uModelMatrix))) * aNormal);
	vFragPos = (uModelMatrix * vec4(aVertex, 1.0)).xyz;
	
	vec3 T = normalize(mat3(uModelMatrix) * aTangent);
	vec3 B = normalize(mat3(uModelMatrix) * cross(aTangent, aNormal));
	vTBN = mat3(T, B, vNormal);
	
	gl_Position = uProjectionMatrix * uViewMatrix * vec4(vFragPos, 1.0);
}



@fragment
#version 330 core

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vFragPos;
in mat3 vTBN;

layout(location=0)out vec3 oPosition;
layout(location=1)out vec3 oNormal;
layout(location=2)out vec3 oAlbedo;
layout(location=3)out vec3 oMRA;
layout(location=4)out vec3 oEmission;

uniform vec4 uAlbedo;
uniform float uMetallic;
uniform float uRoughness;
uniform vec3 uEmission;
uniform sampler2D uNormalMap;
uniform sampler2D uAlbedoMap;
uniform sampler2D uMetallicMap;
uniform sampler2D uRoughnessMap;
uniform sampler2D uAoMap;
uniform sampler2D uEmissionMap;
uniform bool uUseNormalMap;
uniform bool uUseAlbedoMap;
uniform bool uUseMetallicMap;
uniform bool uUseRoughnessMap;
uniform bool uUseAoMap;
uniform bool uUseEmissionMap;

void main() {
	vec3 normal = vNormal;
	vec3 albedo = uAlbedo.xyz;
	float metallic = uMetallic;
	float roughness = uRoughness;
	float ao = 1.0;
	vec3 emission = uEmission;
	if(uUseNormalMap) {
		normal = texture(uNormalMap, vTexCoord).xyz * 2.0 - 1.0;
		normal = normalize(vTBN * normal); //optimize this
	}
	if(uUseAlbedoMap)albedo *= pow(texture(uAlbedoMap, vTexCoord).xyz, vec3(2.2));
	if(uUseMetallicMap)metallic *= texture(uMetallicMap, vTexCoord).x;
	if(uUseRoughnessMap)roughness *= texture(uRoughnessMap, vTexCoord).y;
	if(uUseAoMap)ao *= texture(uAoMap, vTexCoord).z;
	if(uUseEmissionMap)emission += vec3(1.0) - pow(texture(uEmissionMap, vTexCoord).xyz, vec3(2.2));
	
	oPosition = vFragPos;
	oNormal = normal;
	oAlbedo = albedo.xyz;
	oMRA = vec3(metallic, roughness, ao);
	oEmission = emission;
}
