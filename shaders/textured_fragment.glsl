#version 430 core

#define MAX_LIGHTS 10

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;
in mat3 TBN;

uniform sampler2D texture0;
uniform sampler2D normal0;

struct Light {
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

uniform Light lights[MAX_LIGHTS];
uniform int numLights;

out vec4 out_color;

void main()
{
    vec3 norm = texture(normal0, texCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    norm = normalize(TBN * norm);

    vec3 result = vec3(0.0);
    for (int i = 0; i < numLights; ++i) {
        vec3 lightDir = normalize(lights[i].position - fragPos);
        float distance = length(lights[i].position - fragPos);
        float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * distance * distance);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color * attenuation;
        result += diffuse;
    }

    vec4 color = texture(texture0, texCoord);
    out_color = vec4(result * color.rgb, color.a);
}