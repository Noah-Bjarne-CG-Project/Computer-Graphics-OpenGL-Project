#version 330 core


out vec4 FragColor;

// Imports from the Vertex Shader
in vec3 Normal;
in vec3 crntPos;

//uniform vec3 lightPos;
uniform vec4 lightColor;
uniform vec3 camPos;
uniform int amountOfLights;
uniform vec3 pointLightPoses[2];

//point light systeem toevoegen voor extra lichtbronnen in toekomst (standaard code nu)
vec4 oldCodeWorking(){
	// ambient lighting (mess in main zette)
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(pointLightPoses[0] - crntPos);
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

//light with loss of intensity (4. belichting)
vec4 PointLight(vec3 lightningPos){ //Dit ook in terrain?

	vec3 lightVec = lightningPos - crntPos;
	float dist = length(lightVec);
	float a = 0.09f; //(mess in main zette )
	float b = 0.07f; //(same as the above)
	float intensity = 1.0f / (a * dist * dist + b *dist + 1.0f);

	// ambient lighting (mess in main zette)
	float ambient = 0.20f;

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
	//FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient + specular);

	//FragColor = vec4(0.0f, 0.0f, 0.9f, 1.0f) * lightColor * (diffuse + ambient + specular);

	return vec4(0.0f, 0.0f, 0.9f, 1.0f) * lightColor * (diffuse * intensity + ambient + specular * intensity);
}


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
	//FragColor = vec4(0.0f, 0.0f, 0.9f, 1.0f) * lightColor * (diffuse + ambient + specular);
	return vec4(0.0f, 0.0f, 0.9f, 1.0f) * lightColor * (diffuse + ambient + specular);

}

void main()
{
	//FragColor = vec4(0.0f, 0.0f, 0.9f, 1.0f);
	//FragColor = SunLight();
	//FragColor = PointLight();
	//FragColor = PointLight()+SunLight();
	vec4 result = SunLight();
	for(int i = 0; i < amountOfLights; i++)
        result += PointLight(pointLightPoses[i]);   
	FragColor = result;

	
}