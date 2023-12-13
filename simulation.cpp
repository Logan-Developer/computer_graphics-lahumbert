#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "constants.hpp"
#include "shaders.hpp"
#include "camera.hpp"
#include "draw.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

using namespace std;

Camera camera;
Draw draw;

// Global light properties
glm::vec3 lightPos(1.2f, 3.0f, 2.0f);

// Callback function to handle mouse input
void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    camera.calculateFront(xpos, ypos);
}

// Process keyboard input to move the camera
void processInput(GLFWwindow *window)
{
    camera.moveCamera(window);
}

// Function to calculate ambient color based on time of day
glm::vec3 calculateAmbientColor(float timeOfDay)
{
    float ambientIntensity = sin(timeOfDay) * 0.5f + 0.5f;
    glm::vec3 ambientColor = glm::vec3(ambientIntensity, ambientIntensity, ambientIntensity);

    return ambientColor;
}

// Function to calculate ambient color based on time of day
glm::vec3 calculateAmbientColorSky(float timeOfDay)
{
    float ambientIntensity = sin(timeOfDay) * 0.5f + 0.5f;
    glm::vec3 dayColor = glm::vec3(0.53f, 0.81f, 0.98f);
    glm::vec3 nightColor = glm::vec3(0.0f, 0.0f, 0.1f);
    glm::vec3 ambientColor = glm::mix(nightColor, dayColor, ambientIntensity);

    return ambientColor;
}

int main()
{
    srand(time(NULL));

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a GLFW window
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str(), NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Load shaders
    Shaders shaders;
    GLuint shaderProgram = shaders.getShaderProgram();

    // Create a vertex buffer object (VBO) and vertex array object (VAO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    draw.setupCubeTransformations();

    // Load textures
    stbi_set_flip_vertically_on_load(true);
    int width = 0, height = 0, bpp = 0;
    unsigned char *image_data = stbi_load("img/bricks2.jpg", &width, &height, &bpp, 0);

    if (!image_data)
    {
        std::cerr << "Failed to load texture" << std::endl;
        glfwTerminate();
        return -1;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);

    // Set texture parameters and generate mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glActiveTexture(GL_TEXTURE0); // Activate the texture unit first before binding texture

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image_data);

    // Main rendering loop
    float timeOfDay = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        processInput(window);

        timeOfDay += 0.005f;

        glm::vec3 ambientColor = calculateAmbientColor(timeOfDay);
        glm::vec3 ambientColorSky = calculateAmbientColorSky(timeOfDay);

        glClearColor(ambientColorSky.x, ambientColorSky.y, ambientColorSky.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 view = camera.getViewMatrix();

        glUseProgram(shaderProgram);

        GLuint modelMatrixLoc = glGetUniformLocation(shaderProgram, "modelMatrix");
        GLuint viewMatrixLoc = glGetUniformLocation(shaderProgram, "viewMatrix");
        GLuint projectionMatrixLoc = glGetUniformLocation(shaderProgram, "projectionMatrix");

        GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
        GLuint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
        GLuint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");

        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(ambientColor));
        glUniform3fv(viewPosLoc, 1, glm::value_ptr(camera.getCameraPosition()));

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projection));

        for (int i = 0; i < draw.getMainCubeTransformations().size(); i++)
        {
            glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(draw.getMainCubeTransformations()[i]));
            glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view));

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        for (int i = 0; i < draw.getWindowTransformations().size(); i++)
        {
            glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(draw.getWindowTransformations()[i]));
            glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view));

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        // Set texture unit for the sampler uniform
        glUniform1i(glGetUniformLocation(shaderProgram, "gSampler"), 0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}
