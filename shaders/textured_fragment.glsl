#version 430 core

#define MAX_LIGHTS 10

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;
in mat3 TBN;

uniform sampler2D texture0;
uniform sampler2D normal0;

struct Light {
    vec3 direction;
    vec3 color;
};

uniform Light lights[MAX_LIGHTS];
uniform int numLights;

uniform vec3 viewPos;
uniform float shininess;
uniform vec3 ambientLight;

out vec4 out_color;

void main()
{
    vec3 norm = texture(normal0, texCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    norm = normalize(TBN * norm);

    vec3 result = ambientLight;
    for (int i = 0; i < numLights; ++i) {
        vec3 lightDir = normalize(-lights[i].direction);
        
        // Diffuse shading
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color;

        // Specular shading
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = spec * lights[i].color;

        result += diffuse + specular;
    }

    vec4 color = texture(texture0, texCoord);
    out_color = vec4(result * color.rgb, color.a);
}