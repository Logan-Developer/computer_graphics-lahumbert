#version 330 core
in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

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

    FragColor = vec4(result, 1.0);
}
