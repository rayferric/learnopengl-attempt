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
#define GAMMA 2.2
#define EXPOSURE 1.0

in vec2 vTexCoord;

layout(location=5)out vec3 oColor;

uniform sampler2D gColor;

void main() {
	vec3 color = texture(gColor, vTexCoord).xyz;
	
	vec3 mapped = vec3(1.0) - exp(-color * EXPOSURE);
	mapped = pow(mapped, vec3(1.0 / GAMMA));
	
	oColor = mapped;
}
