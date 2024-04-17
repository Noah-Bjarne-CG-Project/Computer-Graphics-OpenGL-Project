#version 330 core

out vec4 FragColor;

// Imports from the Vertex Shader
in vec2 TexCoords;
in vec3 Normal;
in vec3 crntPos;
in float Height;
in vec4 Color;


uniform vec4 lightColor;
uniform vec3 camPos;
uniform sampler2D tex0;

//Directional light. Licht van de zon (enigste nu)
vec4 SunLight(){
	// ambient lighting (mess in main zette)
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f)); //Komt van de zon.
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

	// outputs final color
	//FragColor = texture(tex0, TexCoords) * lightColor * (diffuse + ambient + specular);
	//FragColor = vec4(2.0f, 2.0f, 0.9f, 1.0f) * lightColor * (diffuse + ambient + specular);
	return  texture(tex0, TexCoords) * lightColor * (diffuse + ambient + specular);

}

void main()
{
    //Mess met specular light gaan werken in de toekomst voor mooier detail. maar pas wnr textures finalized zijn
    //FragColor = vec4(h, h, h, 1.0);
    //vec2 texCoords = vec2(1.0f,1.0f);
    //vec4 TexColor = texture(tex0,TexCoords);
    //FragColor = Color * TexColor;
    //FragColor = texture(tex0,TexCoords);

	FragColor = SunLight();

}