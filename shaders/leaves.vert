
#version 410 core
precision highp float;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

out vec2 texCoord;
out vec3 fragPos;

out mat3 TBN;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
    // Calculate the billboarded position
    vec4 billboardPos = model * vec4(vertexPosition, 1.0);

    // Transform the vertex position by the view and projection matrices
    gl_Position = viewProjection * billboardPos;
    texCoord = vertexTexCoord;
    fragPos = billboardPos.xyz;

    vec3 T = normalize(vec3(model * vec4(vertexTangent, 0.0)));
    vec3 B = normalize(vec3(model * vec4(vertexBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(vertexNormal, 0.0)));
    TBN = mat3(T, B, N);
}