#version 120

uniform sampler2D texture0;
uniform vec4 fog_color;

varying vec4 color;
varying vec2 texCoord0;
varying float blendFactor;

void main(void) {
	vec4 fragColor = color * texture2D(texture0, texCoord0.st);	
	gl_FragColor = mix(fog_color, fragColor, blendFactor);
}
