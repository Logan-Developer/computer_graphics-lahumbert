#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord; // Corrected variable name

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D gSampler;

void main()
{
    // Calculate lighting
    vec3 ambient = 0.1 * lightColor; // Ambient lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0); // Diffuse lighting
    vec3 diffuse = diff * lightColor;

    // light gray color for the object
    vec3 result = (ambient + diffuse) * vec3(0.5);

    FragColor = texture(gSampler, TexCoord) * vec4(result, 1.0); // Corrected variable name
}
