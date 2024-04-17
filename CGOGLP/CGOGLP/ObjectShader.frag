#version 330 core


out vec4 FragColor;

// Imports from the Vertex Shader
in vec3 Normal;
in vec3 crntPos;


uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

//point light systeem toevoegen voor extra lichtbronnen in toekomst (standaard code nu)
vec4 oldCodeWorking(){
	// ambient lighting (mess in main zette)
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

	// outputs final color
	//FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient + specular);
	FragColor = vec4(0.0f, 0.0f, 0.9f, 1.0f) * lightColor * (diffuse + ambient + specular);

	//FragColor = vec4(0.0f, 0.0f, 0.9f, 1.0f);
	return vec4(0.0f, 0.0f, 0.9f, 1.0f);
}
//mess spotlight voor moest zaklamp toegevoegd worde idk


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
	//FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient + specular);
	FragColor = vec4(2.0f, 2.0f, 0.9f, 1.0f) * lightColor * (diffuse + ambient + specular);
	return vec4(2.0f, 2.0f, 0.9f, 1.0f) * lightColor * (diffuse + ambient + specular);

}

void main()
{
	FragColor = SunLight();

}