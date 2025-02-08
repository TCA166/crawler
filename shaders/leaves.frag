#version 410 core
precision highp float;

in vec2 texCoord;
in vec3 fragPos;

uniform sampler2D leafTexture;
uniform vec3 ambientLight;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 out_color;

void main()
{
    vec4 leafColor = texture(leafTexture, texCoord);

    // Discard transparent fragments
    if (leafColor.a < 0.5)
        discard;

    // Ambient lighting
    vec3 ambient = ambientLight * leafColor.rgb;

    // Diffuse lighting
    vec3 norm = normalize(fragPos);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * leafColor.rgb;

    // Specular lighting
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = spec * lightColor;

    vec3 result = ambient + diffuse + specular;
    out_color = vec4(result, leafColor.a);
}