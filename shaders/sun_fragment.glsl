#version 430 core

#define MAX_LIGHTS 10

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;
in mat3 TBN;

out vec4 out_color;

void main()
{
    out_color = vec4(vec3(1.0), 1.0);
}