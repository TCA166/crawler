#version 430 core

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;
in mat3 TBN;

uniform sampler2D texture0;

uniform vec3 viewPos;
uniform float shininess;
uniform vec3 ambientLight;

out vec4 out_color;

void main()
{
    vec4 color = texture(texture0, texCoord);
    out_color = vec4(color.rgb, color.a);
}