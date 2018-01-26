#version 120

uniform samplerCube texture0;

varying vec3 texCoord0;

void main(void) {
	gl_FragColor = vec4(0.5, 0.5, 0.5, 0.5) + textureCube(texture0, texCoord0);
}
