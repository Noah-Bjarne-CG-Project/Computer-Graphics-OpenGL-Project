#version 330 core

out vec4 FragColor;

uniform vec4 lightColor;

void main()
{
	FragColor = lightColor;
	//FragColor = vec4(4.0f, 0.0f, 0.9f, 1.0f);
}
