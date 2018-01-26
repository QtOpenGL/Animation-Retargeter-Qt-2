#version 130

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;
uniform vec3 camera_position;

invariant in vec3 a_Vertex;
in vec2 a_TexCoord0;
in vec3 a_Normal;

out vec3 texCoord0;

void main(void) 
{
	vec4 pos = modelview_matrix * vec4(a_Vertex, 1.0);	
	gl_Position = projection_matrix * pos;	

	vec3 view = a_Vertex.xyz - camera_position.xyz;
	texCoord0 = -reflect(view, a_Normal);
}
