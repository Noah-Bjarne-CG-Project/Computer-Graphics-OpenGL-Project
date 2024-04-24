#version 330 core

out vec4 FragColor;

// Imports from the Vertex Shader
in vec2 TexCoords;
in vec3 Normal;
in vec3 crntPos;


uniform vec4 lightColor;
uniform vec3 camPos;
uniform sampler2D texture_diffuse1;
uniform int amountOfLights;
uniform vec3 pointLightPoses[2];

vec4 PointLight(vec3 lightningPos){

	vec3 lightVec = lightningPos - crntPos;
	float dist = length(lightVec);
	float a = 0.09f; //(mess in main zette )
	float b = 0.07f; //(same as the above)
	float intensity = 1.0f / (a * dist * dist + b *dist + 1.0f);

	// ambient lighting (mess in main zette)
	float ambient = 0.00f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

	// outputs final color
	//FragColor = texture(ttexture_diffuse1, TexCoords) * lightColor * (diffuse * intensity + ambient + specular * intensity);


	//return vec4(0.0f, 0.0f, 0.9f, 1.0f) * lightColor * (diffuse * intensity + ambient + specular * intensity);
	return texture(texture_diffuse1, TexCoords) * lightColor * (diffuse * intensity + ambient + specular * intensity);

}


//Directional light. Licht van de zon (enigste nu)
vec4 SunLight(){
	//spectral map toevoegen maybe
	// ambient lighting (mess in main zette)
	float ambient = 0.10f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f)); //Komt van de zon.
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

	// outputs final color
	return  texture(texture_diffuse1, TexCoords) * lightColor * (diffuse + ambient + specular);

}

void main()
{
	vec4 result = SunLight();
	//vec4 result = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	for(int i = 0; i < 2; i++)
        result += PointLight(pointLightPoses[i]);   
	FragColor = result;

}