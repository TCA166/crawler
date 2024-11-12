#version 430 core

#define MAX_LIGHTS 10

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;
in mat3 TBN;

uniform sampler2D ground;
uniform sampler2D night;
uniform sampler2D clouds;
uniform sampler2D ground_normal;

struct Light {
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

uniform Light lights[MAX_LIGHTS];
uniform int numLights;

uniform vec3 viewPos;
uniform float shininess;
uniform vec3 ambientLight;

out vec4 out_color;

void main()
{
    vec3 norm = texture(ground_normal, texCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    norm = normalize(TBN * norm);

    vec3 result = ambientLight;
    for (int i = 0; i < numLights; ++i) {
        vec3 lightDir = normalize(lights[i].position - fragPos);
        // Check if the light should affect the fragment
        float lightEffect = dot(lightDir, norm);
        if (lightEffect <= 0.0) {
            continue;
        }
        float distance = length(lights[i].position - fragPos);
        float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * distance * distance);

        // Diffuse shading
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color * attenuation;

        // Specular shading
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
        vec3 specular = spec * lights[i].color * attenuation;

        result += diffuse + specular;
    }
    if (texCoord.y < 0.5) {
        out_color = texture(night, texCoord);
    } else {
        vec4 color = texture(ground, texCoord);
        out_color = vec4(result * color.rgb, color.a);
    }
}