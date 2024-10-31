#version 430 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;	
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

out vec2 texCoord;
out mat3 TBN;
out vec3 fragPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
    // Transform the vertex position by the model and view-projection matrices
    gl_Position = viewProjection * model * vec4(vertexPosition, 1.0);
    texCoord = vertexTexCoord;

    // Transform the fragment position by the model matrix only
    fragPos = vec3(model * vec4(vertexPosition, 1.0));

    // Transform the normal by the model matrix only
    normal = mat3(model) * vertexNormal;

    // Calculate the TBN matrix using the model matrix only
    vec3 T = normalize(mat3(model) * vertexTangent);
    vec3 B = normalize(mat3(model) * vertexBitangent);
    vec3 N = normalize(mat3(model) * vertexNormal);
    TBN = mat3(T, B, N);
}