#version 410 core
precision highp float;

#define MAX_LIGHTS 10
const int smoothing_window = 1;

in vec2 texCoord;
in vec3 fragPos;
in mat3 TBN;

uniform sampler2D texture0;
uniform sampler2D normal0;

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

vec3 CalcLight(Light light, vec3 norm)
{
    vec3 light_distance = light.position - fragPos;
    float distance = length(light_distance);
    vec3 lightDir = normalize(light_distance);
        
    // Diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;
    // Specular shading
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = spec * light.color;

    // Shadow calculation
    vec4 fragPosLightSpace = light.lightSpaceMatrix * vec4(fragPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    // very important sanity checks THAT NOBODY TOLD ME ABOUT
    if (projCoords.z > 1.0 || projCoords.z < 0.0 || projCoords.x > 1.0 || projCoords.x < 0.0 || projCoords.y > 1.0 || projCoords.y < 0.0) {
        return vec3(0.0);
    }

    float currentDepth = projCoords.z;
    float bias = max(0.01 * (1.0 - dot(norm, lightDir)), 0.05);
    float shadow = 0.0;
    vec2 texelSize = vec2(textureSize(light.depthMap, 0));
    texelSize.x = 1.0 / texelSize.x;
    texelSize.y = 1.0 / texelSize.y;
    for(int x = -smoothing_window; x <= smoothing_window; ++x)
    {
        for(int y = -smoothing_window; y <= smoothing_window; ++y)
        {
            vec2 coord = projCoords.xy + vec2(x, y) * texelSize;
            // check if the current pixel is in bounds of the shadow map
            float pcfDepth = texture(light.depthMap, coord).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;    
        }    
    }
    shadow /= pow(smoothing_window * 2 + 1, 2);

    float attenuation = 1.0 / (1 + 0.09 * distance + 0.032 * distance * distance);
    attenuation *= max(0.0, 1.0 - distance / light.range); // Ensure light falls off to zero at the maximum range

    return (1.0 - shadow) * (diffuse + specular) * attenuation;
}

void main()
{
    vec3 norm = texture(normal0, texCoord).rgb;
    norm = normalize(TBN * (norm * 2.0 - 1.0));

    vec3 result = ambientLight;
    for (int i = 0; i < numLights; ++i) {
        result += CalcLight(lights[i], norm);
    }

    vec4 color = texture(texture0, texCoord);
    out_color = vec4(result * color.rgb, color.a);
}