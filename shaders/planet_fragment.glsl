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
uniform sampler2D spec;

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
uniform float time;

out vec4 out_color;

float cloud_move_speed = 0.001;

float sunset_threshold = 0.1;

vec3 calculate_light(vec3 norm, Light light){
    vec3 lightDir = normalize(light.position - fragPos);
    // Check if the light should affect the fragment
    float lightEffect = dot(lightDir, norm);
    if (lightEffect <= 0.0) {
        return vec3(0.0);
    }
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    // Diffuse shading
    float diff = max(lightEffect, 0.0);
    vec3 diffuse = diff * light.color * attenuation;

    // Specular shading
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specMap = texture(spec, texCoord).r;
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 1.0) * specMap;
    vec3 specular = spec * light.color;

    return diffuse + specular;
}

void main()
{
    vec3 norm = texture(ground_normal, texCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    norm = normalize(TBN * norm);
    //lighting of the ground
    vec3 result = ambientLight;
    for (int i = 0; i < numLights; ++i) {
        result += calculate_light(norm, lights[i]);
    }
    vec4 color;
    if(result == ambientLight) { // Night
        color = texture(night, texCoord);
    } else if (result.x < sunset_threshold && result.y < sunset_threshold && result.z < sunset_threshold) { // Sunset
        color = mix(texture(ground, texCoord), texture(night, texCoord), 0.5);
    } else { // Day    
        color = texture(ground, texCoord);
    }
    // simplified cloud lighting
    vec3 cloud_result = ambientLight;
    for (int i = 0; i < numLights; ++i) {
        cloud_result += calculate_light(vec3(0.0, 0.0, 1.0), lights[i]);
    }
    vec4 cloud_color = texture(clouds, texCoord + vec2(time * cloud_move_speed, time * cloud_move_speed));
    cloud_color.rgb *= cloud_result; // Apply lighting to clouds
    out_color = vec4(result * out_color.rgb, out_color.a);
    out_color = mix(color, cloud_color, 0.5);
}