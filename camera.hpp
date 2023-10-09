#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
    glm::vec3 cameraPosition;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    GLfloat yaw;
    GLfloat pitch;

    GLfloat lastX;
    GLfloat lastY;

public:
    Camera()
    {
        this->cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f); // it represents the position of the camera in the world
        this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);   // it represents the direction the camera is pointing at
        this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);       // it represents the up vector of the camera

        this->yaw = -90.0f;
        this->pitch = 0.0f;

        this->lastX = WINDOW_WIDTH / 2.0f;
        this->lastY = WINDOW_HEIGHT / 2.0f;
    }

    void calculateFront(int xpos, int ypos)
    {
        GLfloat xoffset = xpos - lastX;
        GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates range from bottom to top

        lastX = xpos;
        lastY = ypos;

        xoffset *= CAMERA_SPEED;
        yoffset *= CAMERA_SPEED;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > PITCH_CONSTRAINT)
            pitch = PITCH_CONSTRAINT;
        if (pitch < -PITCH_CONSTRAINT)
            pitch = -PITCH_CONSTRAINT;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
    }

    void moveCamera(GLFWwindow *window)
    {
        float cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            cameraSpeed = CAMERA_SPEED_FAST;
        else
            cameraSpeed = CAMERA_SPEED;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPosition += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPosition -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }

    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    }

    glm::vec3 getCameraPosition()
    {
        return cameraPosition;
    }
};
