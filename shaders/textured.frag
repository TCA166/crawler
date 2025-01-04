#version 430 core

#define MAX_LIGHTS 10
#define bias 0.005

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;
in mat3 TBN;

uniform sampler2D texture0;
uniform sampler2D normal0;

struct Light {
    vec3 direction;
    vec3 color;
    mat4 lightSpaceMatrix;
    sampler2D shadowMap;
};

uniform Light lights[MAX_LIGHTS];
uniform int numLights;

uniform vec3 viewPos;
uniform float shininess;
uniform vec3 ambientLight;

out vec4 out_color;

vec3 CalcDirectionalLight(Light light, vec3 norm)
{
    vec3 lightDir = normalize(-light.direction);
        
    // Diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    // Specular shading
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * light.color;

    // Shadow calculation
    vec4 fragPosLightSpace = light.lightSpaceMatrix * vec4(fragPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(light.shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return (1.0 - shadow) * (diffuse + specular);
}

void main()
{
    vec3 norm = texture(normal0, texCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    norm = normalize(TBN * norm);

    vec3 result = ambientLight;
    for (int i = 0; i < numLights; ++i) {
        result += CalcDirectionalLight(lights[i], norm);
    }

    vec4 color = texture(texture0, texCoord);
    out_color = vec4(result * color.rgb, color.a);
}