#version 330 core

layout(location = 0) in vec3 inPosition;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
void main()
{
    mat4 mvpMatrix = projectionMatrix * viewMatrix;
    gl_Position = mvpMatrix * vec4(inPosition, 1.0);
}
