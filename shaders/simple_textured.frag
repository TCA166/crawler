#version 410 core
precision highp float;

in vec2 texCoord;

uniform sampler2D texture0;

out vec4 out_color;

void main()
{
    out_color = texture(texture0, texCoord);
}