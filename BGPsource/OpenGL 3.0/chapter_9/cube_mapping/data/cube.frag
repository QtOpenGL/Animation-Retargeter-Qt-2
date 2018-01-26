#version 130

precision highp float;

uniform samplerCube texture0;

in vec3 texCoord0;
out vec4 outColor;

void main(void) {
	outColor = vec4(0.5, 0.5, 0.5, 0.5) + texture(texture0, texCoord0);
}
