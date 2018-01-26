#version 120

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;

uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform vec4 material_emissive;
uniform float material_shininess;

uniform struct {
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	vec4 ambient;

	float constant_attenuation;
	float linear_attenuation;
	float quadratic_attenuation;
} lights[2];

attribute vec3 a_Vertex;
attribute vec2 a_TexCoord0;
attribute vec3 a_Normal;

varying vec4 color;
varying vec2 texCoord0;

void main(void) 
{
    vec3 N = normalize(normal_matrix * a_Normal);	
    vec4 pos = modelview_matrix * vec4(a_Vertex, 1.0);

    //Find the eye vector (same as a directional light)
    vec3 E = -(pos.xyz);
	
    //Calculate the light position in eye space by multiplying by the modelview matrix
    
    //Get the light direction vector by finding the 
    //vector between the vertex and light position
	
	
	vec4 tempColor = vec4(0.0);

	for (int i = 0; i < 2; ++i) {
		vec3 lightPos = (modelview_matrix * lights[i].position).xyz;
		vec3 lightDir = (lightPos - pos.xyz).xyz;

    		//Find the angle between the normal and the light direction
    		float NdotL = max(dot(N, lightDir.xyz), 0.0);
    	
		//The distance between the vertex and the light is the length
    		//of the light direction vector
	    	float dist = length(lightDir);

    		//Assign the ambient color
    		vec4 lightColor = material_ambient * lights[i].ambient;

	    	if (NdotL > 0.0) 
    		{
        		vec3 HV = normalize(lightPos + E);
	        	lightColor += material_diffuse * lights[i].diffuse * NdotL;

	  		float NdotHV = max(dot(N, HV), 0.0);
      	  	lightColor += material_specular * lights[i].specular * pow(NdotHV, material_shininess);	
	    	}
	
		//Calculate the attenuation factor
		float attenuation = 1.0 / (lights[i].constant_attenuation + 
                                   lights[i].linear_attenuation * dist + 
                                   lights[i].quadratic_attenuation * dist * dist);

		//The material emissive value isn't affected by attenuation so that is added separately
		tempColor += (lightColor * attenuation);
	}

	color = material_emissive + tempColor;
	texCoord0 = a_TexCoord0;
	gl_Position = projection_matrix * pos;	
}
