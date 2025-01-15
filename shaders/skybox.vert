#version 410 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;	

out vec3 TexCoords;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
    vec4 pos = viewProjection * model * vec4(vertexPosition, 1.0);
    TexCoords = vertexPosition;
    gl_Position = pos.xyww;
}  