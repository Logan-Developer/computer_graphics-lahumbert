#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 TexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec3 TexCoord0; // Corrected variable name

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    FragPos = vec3(modelMatrix * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(modelMatrix))) * aPos; // Transform normal to world space
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
    TexCoord0 = TexCoord;
}
