#version 120

uniform sampler2D texture0;

varying vec2 texCoord0;

void main(void) {
	vec4 outColor = texture2D(texture0, texCoord0.st);
	if (outColor.rgb == vec3(0.0, 0.0, 0.0)) {
		discard;
	}
	
	gl_FragColor = outColor;
}
