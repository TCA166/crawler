#version 410 core
precision highp float;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

uniform sampler2D normal0;

out vec2 texCoord;
out vec3 fragPos;

out vec3 viewPos;
out vec3 shininess;

out mat3 TBN;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
    // Transform the vertex position by the model and view-projection matrices
    gl_Position = viewProjection * model * vec4(vertexPosition, 1.0);
    texCoord = vertexTexCoord;

    // Transform the fragment position by the model matrix only
    fragPos = vec3(model * vec4(vertexPosition, 1.0));

    // Calculate the TBN matrix
    vec3 T = normalize(vec3(model * vec4(vertexTangent, 0.0)));
    vec3 B = normalize(vec3(model * vec4(vertexBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(vertexNormal, 0.0)));
    TBN = mat3(T, B, N);
}