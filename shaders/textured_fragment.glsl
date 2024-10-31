#version 430 core

in vec2 texCoord;
in mat3 TBN;
uniform sampler2D texture0;
uniform sampler2D normal0;
out vec4 out_color;

void main()
{
	
	// Sample the normal from the normal map
    vec3 normal = texture(normal0, texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0); // Transform from [0, 1] to [-1, 1]

    // Transform the normal to world space
    normal = normalize(TBN * normal);

    // Sample the diffuse color
    vec4 color = texture(texture0, texCoord);

    // Simple lighting calculation (example)
    vec3 lightDir = normalize(vec3(0.0, 0.0, 1.0)); // Light direction
    float diff = max(dot(normal, lightDir), 0.0);

    // Set the fragment color
    out_color = vec4(color.rgb * diff, color.a);
	//out_color = vec4(1.0, 0.0, 0.0, 1.0);
}