
#version 410 core
precision highp float;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;
layout(location = 5) in vec3 offset;

out vec2 texCoord;
out vec3 fragPos;

out mat3 TBN;

uniform mat4 model;
uniform mat4 viewProjection;
uniform vec3 viewPos;

void main()
{
    vec3 worldPosition = vertexPosition + offset;
    vec3 toCamera = normalize(viewPos - worldPosition);
    vec3 right = normalize(cross(vec3(0.0, 1.0, 0.0), toCamera));
    vec3 up = cross(toCamera, right);

    mat4 billboardMatrix = model * mat4(
        vec4(right, 0.0),
        vec4(up, 0.0),
        vec4(toCamera, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );

    // Calculate the billboarded position
    vec4 billboardPos = billboardMatrix * vec4(vertexPosition, 1.0);

    // Transform the vertex position by the view and projection matrices
    gl_Position = viewProjection * (billboardPos + vec4(offset, 0.0));
    texCoord = vertexTexCoord;
    fragPos = (billboardPos + vec4(offset, 0.0)).xyz;

    vec3 T = normalize(vec3(billboardMatrix * vec4(vertexTangent, 0.0)));
    vec3 B = normalize(vec3(billboardMatrix * vec4(vertexBitangent, 0.0)));
    vec3 N = normalize(vec3(billboardMatrix * vec4(vertexNormal, 0.0)));
    TBN = mat3(T, B, N);
}