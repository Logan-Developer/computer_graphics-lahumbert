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

using namespace std;

Camera camera;
Draw draw;

// Global light properties
glm::vec3 lightPos(LIGHT_POS_X, LIGHT_POS_Y, LIGHT_POS_Z);

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
    float ambientIntensity = sin(timeOfDay) * LIGHT_AMBIENT_INTENSITY + LIGHT_AMBIENT_INTENSITY;
    glm::vec3 ambientColor = glm::vec3(ambientIntensity, ambientIntensity, ambientIntensity);

    return ambientColor;
}

// Function to calculate ambient color based on time of day
glm::vec3 calculateAmbientColorSky(float timeOfDay)
{
    // Adjust these values to achieve the desired color changes
    float ambientIntensity = sin(timeOfDay) * LIGHT_AMBIENT_INTENSITY + LIGHT_AMBIENT_INTENSITY;

    // Interpolate between sky blue during the day and dark color at night
    glm::vec3 dayColor = glm::vec3(0.53f, 0.81f, 0.98f); // Sky blue color
    glm::vec3 nightColor = glm::vec3(0.0f, 0.0f, 0.1f);  // Dark color

    glm::vec3 ambientColor = glm::mix(nightColor, dayColor, ambientIntensity);

    return ambientColor;
}

int main()
{
    // init rand
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

    // Main rendering loop
    float timeOfDay = 0.0f; // Initialize time of day
    while (!glfwWindowShouldClose(window))
    {
        // Poll for and process events
        glfwPollEvents();

        // Process keyboard input to move the camera
        processInput(window);

        // Update time of day
        timeOfDay += TIME_OF_DAY_INCREMENT;

        // Calculate ambient light color based on time of day
        glm::vec3 ambientColor = calculateAmbientColor(timeOfDay);
        glm::vec3 ambientColorSky = calculateAmbientColorSky(timeOfDay);

        // Clear the screen and enable depth testing
        glClearColor(ambientColorSky.x, ambientColorSky.y, ambientColorSky.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // Set up the view matrix (user-controlled view)
        glm::mat4 view = camera.getViewMatrix();

        // Use the shader program
        glUseProgram(shaderProgram);

        // Get the location of the model, view, and projection matrices uniforms in the shader
        GLuint modelMatrixLoc = glGetUniformLocation(shaderProgram, "modelMatrix");
        GLuint viewMatrixLoc = glGetUniformLocation(shaderProgram, "viewMatrix");
        GLuint projectionMatrixLoc = glGetUniformLocation(shaderProgram, "projectionMatrix");

        // Get the location of light uniforms in the shader
        GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
        GLuint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
        GLuint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");

        // Set light uniforms
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(ambientColor));
        glUniform3fv(viewPosLoc, 1, glm::value_ptr(camera.getCameraPosition()));

        // Set up the projection matrix (perspective projection)
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

        // Set the projection matrix (perspective projection)
        glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Draw the main cubes
        for (int i = 0; i < draw.getMainCubeTransformations().size(); i++)
        {
            glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(draw.getMainCubeTransformations()[i]));
            glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view));

            // Draw the cube
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        // Draw the windows
        for (int i = 0; i < draw.getWindowTransformations().size(); i++)
        {
            glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(draw.getWindowTransformations()[i]));
            glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view));

            // Draw the cube
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Clean up and exit
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}
