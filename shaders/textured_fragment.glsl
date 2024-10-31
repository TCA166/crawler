#version 430 core

uniform vec3 objectColor;
in vec3 normal;
out vec4 FragColor;
in vec2 texCoord;

void main()
{
	FragColor = vec4(normal, 1.0);
}