#version 410 core
precision highp float;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;	

uniform mat4 viewProjection;
uniform mat4 model;

void main()
{
    gl_Position = viewProjection * model * vec4(vertexPosition, 1.0);
}  