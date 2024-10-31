#version 430 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;	
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

out vec2 texCoord;
out mat3 TBN;

uniform mat4 transformation;

void main()
{
	gl_Position = transformation * vec4(vertexPosition, 1.0);
	texCoord = vertexTexCoord;

	// Calculate the TBN matrix
    vec3 T = normalize(mat3(transformation) * vertexTangent);
    vec3 B = normalize(mat3(transformation) * vertexBitangent);
    vec3 N = normalize(mat3(transformation) * vertexNormal);
    TBN = mat3(T, B, N);
}