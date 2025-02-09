#version 410 core
precision highp float;

in vec2 texCoord;

uniform sampler2D texture0;

out vec4 out_color;

void main()
{
    vec4 color = texture(texture0, texCoord);
    if (color.a <= 0.0) {
        discard;
    }
    out_color = color;
}