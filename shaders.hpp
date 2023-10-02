#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Shader
{
    string filename;
    GLenum shaderType;
    GLuint shader;

public:
    Shader(string filename, GLenum shaderType)
    {
        this->filename = filename;
        this->shaderType = shaderType;
        this->shader = 0;
    }

    string getFilename()
    {
        return filename;
    }

    GLenum getShaderType()
    {
        return shaderType;
    }

    GLuint getShader()
    {
        return shader;
    }

    void setShader(GLuint shader)
    {
        this->shader = shader;
    }
};

class Shaders
{
    vector<Shader> shaders;
    GLuint shaderProgram;

    void loadShaders()
    {
        for (Shader &shader : shaders)
        {
            GLuint theShader = glCreateShader(shader.getShaderType());
            char *buffer;
            fstream in;

            buffer = (char *)malloc(10000);
            in.open(shader.getFilename().c_str(), ios::in);
            in.read(buffer, 10000);
            buffer[in.gcount()] = 0;
            in.close();
            glShaderSource(theShader, 1, (const char **)&buffer, NULL);
            free(buffer);
            glCompileShader(theShader);

            // Check for vertex shader compilation errors
            GLint success;
            glGetShaderiv(theShader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                GLchar infoLog[512];
                glGetShaderInfoLog(theShader, 512, NULL, infoLog);
                std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
            }

            shader.setShader(theShader);
        }
    }

public:
    Shaders()
    {
        shaders.push_back(Shader("shaders/vertex/camera.vert.glsl", GL_VERTEX_SHADER));
        
        shaders.push_back(Shader("shaders/fragment/cubecolor.frag.glsl", GL_FRAGMENT_SHADER));
        loadShaders();

        // Create the shader program and link the vertex shader
        shaderProgram = glCreateProgram();
        for (Shader shader : shaders)
        {
            glAttachShader(shaderProgram, shader.getShader());
        }

        glLinkProgram(shaderProgram);

        // Check for shader program linkage errors
        GLint success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            GLchar infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cerr << "Shader program linkage failed: " << infoLog << std::endl;
        }

        // Delete the shaders as they're linked into our program now and no longer necessery
        for (Shader shader : shaders)
        {
            glDeleteShader(shader.getShaderType());
        }
    }

    ~Shaders()
    {
        glDeleteProgram(shaderProgram);
    }

    GLuint getShaderProgram()
    {
        return shaderProgram;
    }
};