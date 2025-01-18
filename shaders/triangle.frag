#version 410 core
precision highp float;

#define MAX_LIGHTS 10

in vec2 texCoord;
in vec3 fragPos;

uniform sampler2D texture0;
uniform sampler2D normal0;


struct Light {
    vec3 position;
    vec3 color;
    mat4 lightSpaceMatrix;
    sampler2D depthMap;
};

uniform Light lights[MAX_LIGHTS];
uniform int numLights;

uniform vec3 viewPos;
uniform float shininess;
uniform vec3 ambientLight;

out vec4 out_color;

vec3 CalcDirectionalLight(Light light, vec3 norm)
{
    vec3 lightDir = normalize(light.position - fragPos);
        
    // Diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = spec * light.color;

    // Shadow calculation
    vec4 fragPosLightSpace = light.lightSpaceMatrix * vec4(fragPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float currentDepth = projCoords.z;
    float bias = max(0.005 * (1.0 - dot(norm, lightDir)), 0.005);
    float shadow = 0.0;
    vec2 texelSize = vec2(textureSize(light.depthMap, 0));
    texelSize.x = 1.0 / texelSize.x;
    texelSize.y = 1.0 / texelSize.y;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(light.depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    return (1.0 - shadow) * (diffuse + specular);
}

void main()
{
    vec3 norm = texture(normal0, texCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);

    vec3 result = ambientLight;
    for (int i = 0; i < numLights; ++i) {
        result += CalcDirectionalLight(lights[i], norm);
    }

    vec4 color = texture(texture0, texCoord);
    
    // Enhance color saturation and brightness
    vec3 enhancedColor = color.rgb * 1.2; // Increase brightness
    enhancedColor = clamp(enhancedColor, 0.0, 1.0); // Ensure values are within valid range

    out_color = vec4(result * enhancedColor, color.a);
}

