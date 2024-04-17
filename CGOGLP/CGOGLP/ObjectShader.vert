#version 330 core
//position
layout (location = 0) in vec3 aPos;
//Normals
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 camMatrix;

//not needed anymore?
uniform mat4 view;
uniform mat4 projection;


out vec3 Normal;
out vec3 crntPos;


void main()
{
	
	crntPos = vec3(model * vec4(aPos, 1.0f));
	//gl_Position = camMatrix * vec4(crntPos, 1.0);
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	Normal = aNormal;
	
}