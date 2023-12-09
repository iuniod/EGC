#version 330

// Input
in vec3 frag_normal;
in vec2 tex_coord;

// Uniform properties
uniform sampler2D texture_1;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
	// Parameters
	vec3 ambientColor = vec3(1.000000, 1.000000, 1.000000); // Ka
	vec3 specularColor = vec3(0.500000, 0.500000, 0.500000); // Ks
	vec3 emissiveColor = vec3(0.000000, 0.000000, 0.000000); // Ke
	float shininess = 0.000000; // Ns
	float refractiveIndex = 1.450000; // Ni
	float transparency = 1.000000; // d
	int illuminationModel = 2; // illum

	vec4 texColor = texture(texture_1, tex_coord);

	vec3 ambient = ambientColor * texColor.rgb;
	vec3 diffuse = vec3(0.162642, 0.090861, 0.040243) * texColor.rgb;
	vec3 specular = specularColor * texColor.rgb;

	out_color = vec4(ambient + diffuse + specular, 1.0);
}
