#version 410 core
precision highp float;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;	

out vec2 texCoord;
out vec3 fragPos;

out vec3 viewPos;
out vec3 shininess;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
    // Transform the vertex position by the model and view-projection matrices
    gl_Position = viewProjection * model * vec4(vertexPosition, 1.0);
    texCoord = vertexTexCoord;

    // Transform the fragment position by the model matrix only
    fragPos = vec3(model * vec4(vertexPosition, 1.0));
}