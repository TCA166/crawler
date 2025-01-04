#version 430 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;	
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

out vec3 TexCoords;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
    vec4 pos = viewProjection * model * vec4(vertexPosition, 1.0);
    TexCoords = vertexPosition;
    gl_Position = pos.xyww;
}  