#version 410 core
precision highp float;

#define MAX_LIGHTS 10
const int smoothing_window = 1;

in vec2 texCoord;
in vec3 fragPos;
in mat3 TBN;

uniform sampler2D leafTexture;

struct Light {
    vec3 position;
    vec3 color;
    float range;
    mat4 lightSpaceMatrix;
    sampler2D depthMap;
};

uniform Light lights[MAX_LIGHTS];
uniform int numLights;

uniform vec3 viewPos;
uniform float shininess;
uniform vec3 ambientLight;

out vec4 out_color;

vec3 CalcLight(Light light)
{
    vec3 light_distance = light.position - fragPos;
    float distance = length(light_distance);
    vec3 lightDir = normalize(light_distance);

    // Shadow calculation
    vec4 fragPosLightSpace = light.lightSpaceMatrix * vec4(fragPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    // very important sanity checks THAT NOBODY TOLD ME ABOUT
    if (projCoords.z > 1.0 || projCoords.z < 0.0 || projCoords.x > 1.0 || projCoords.x < 0.0 || projCoords.y > 1.0 || projCoords.y < 0.0) {
        return vec3(0.0);
    }

    float currentDepth = projCoords.z;
    float bias = max(0.01 * (1.0 - dot(normalize(fragPos - light.position), lightDir)), 0.05);
    float shadow = texture(light.depthMap, projCoords.xy).r;
    shadow = currentDepth - bias > shadow ? 1.0 : 0.0;

    float attenuation = 1.0 / (1 + 0.09 * distance + 0.032 * distance * distance);
    attenuation *= max(0.0, 1.0 - distance / light.range); // Ensure light falls off to zero at the maximum range

    return (1.0 - shadow) * (light.color) * attenuation;
}

void main()
{
    vec4 leafColor = texture(leafTexture, texCoord);

    // Discard transparent fragments
    if (leafColor.a < 0.5)
        discard;

    vec3 result = ambientLight;
    for (int i = 0; i < numLights; ++i) {
        result += CalcLight(lights[i]);
    }

    out_color = vec4(result * leafColor.rgb, leafColor.a);
}