#version 130

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;

uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform vec4 material_emissive;
uniform float material_shininess;

struct light {
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	vec4 ambient;
	
	float constant_attenuation;
	float linear_attenuation;
	float quadratic_attenuation;

	float spotCutOff;
	vec3 spotDirection;
	float spotExponent;
};

uniform light light0;

in vec3 a_Vertex;
in vec2 a_TexCoord0;
in vec3 a_Normal;

out vec4 color;
out vec2 texCoord0;

void main(void) 
{
	vec3 N = normalize(normal_matrix * a_Normal);	
	vec3 lightPos = (modelview_matrix * light0.position).xyz;
	vec4 pos = modelview_matrix * vec4(a_Vertex, 1.0);
	vec3 lightDir = (lightPos - pos.xyz).xyz;

	float NdotL = max(dot(N, lightDir.xyz), 0.0);
	float dist = length(lightDir);

	vec3 E = -(pos.xyz);

	vec4 finalColor = material_ambient * light0.ambient;

	float attenuation = 1.0;
	
      //If the surface is facing the light source
	if (NdotL > 0.0) 
	{
            //Find the angle between the light direction and spotlight direction
		float spotEffect = dot(normalize(light0.spotDirection), normalize(-lightDir));

		//If it's greater than the cosine of the spotlight cutoff then it should be illuminated
		if (spotEffect > cos(light0.spotCutOff)) 
		{	
			vec3 HV = normalize(lightPos + E);
			float NdotHV = max(dot(N, HV), 0.0);
			finalColor += material_specular * light0.specular * pow(NdotHV, material_shininess);	
		
			spotEffect = pow(spotEffect, light0.spotExponent);		
			attenuation = spotEffect / (light0.constant_attenuation + light0.linear_attenuation * dist + light0.quadratic_attenuation * dist * dist);

			finalColor += material_diffuse * light0.diffuse * NdotL;		
		}
	}
	
	color = material_emissive + (finalColor * attenuation);

	texCoord0 = a_TexCoord0;

	gl_Position = projection_matrix * pos;	
}
