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

in vec3 vFragPos;

layout(location=0)out vec3 oAlbedo;

uniform sampler2D uEquirectMap;

vec2 sphericalProj(vec3 v) {
    return vec2(atan(v.z, v.x), asin(v.y)) * vec2(0.1591, 0.3183) + vec2(0.5);
}

void main() {
	vec2 texCoord = sphericalProj(normalize(vFragPos));
	oAlbedo = texture(uEquirectMap, texCoord).xyz;
}
