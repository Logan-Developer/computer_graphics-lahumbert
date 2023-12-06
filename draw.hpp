// Define the 3D object's vertices (a cube)
GLfloat vertices[] = {
    -1.0f, -1.0f, -1.0f, // triangle 1 : begin
    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f, -1.0f, // triangle 2 : begin
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f, // triangle 2 : end
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f};

class Draw
{
    std::vector<glm::mat4> mainCubeTransformations;
    std::vector<glm::mat4> windowTransformations;

public:
    void setupCubeTransformations()
    {
        int numCubes = (rand() % NUM_CUBES_MAX) + NUM_CUBES_MIN; // Random number of cubes
        int y = 0;                                               // Set the Y-coordinate to a common ground level

        // Before the rendering loop, generate transformation matrices for cubes with variations
        for (int i = 0; i < numCubes; i++)
        {
            // cube position ranges from CUBE_POS_MIN to CUBE_POS_MAX
            int x = static_cast<int>((static_cast<float>(rand()) / RAND_MAX) * CUBE_POS_MAX) + CUBE_POS_MIN;
            int z = static_cast<int>((static_cast<float>(rand()) / RAND_MAX) * CUBE_POS_MAX) + CUBE_POS_MIN;

            glm::mat4 modelMatrix = glm::mat4(1.0f);

            // Translate the main cube to the current position
            modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));

            // Rotate the main cube to a random angle between CUBE_ROT_MIN and CUBE_ROT_MAX
            modelMatrix = glm::rotate(modelMatrix, glm::radians(static_cast<float>(rand()) / RAND_MAX * (CUBE_ROT_MAX - CUBE_ROT_MIN) + CUBE_ROT_MIN), glm::vec3(0.0f, 1.0f, 0.0f));

            // Scale the main cube to a random size between CUBE_SCALE_MIN and CUBE_SCALE_MAX
            modelMatrix = glm::scale(modelMatrix, glm::vec3(static_cast<float>(rand()) / RAND_MAX * (CUBE_SCALE_MAX - CUBE_SCALE_MIN) + CUBE_SCALE_MIN));

            // Store the transformation matrix for the main cube
            mainCubeTransformations.push_back(modelMatrix);

            // Iterate over each face of the main cube to add random windows
            for (int windowIndex = 0; windowIndex < NUM_WINDOWS; windowIndex++)
            {
                // Generate random positions for windows relative to the main cube
                float dx = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
                float dz = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;

                // Translate and scale to create a smaller cube (window) relative to the main cube
                glm::mat4 windowMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(dx, 0.0f, dz));
                windowMatrix = glm::scale(windowMatrix, glm::vec3(0.2f, 0.2f, 0.2f));

                // Apply the window transformation to the main cube's position
                glm::mat4 finalWindowMatrix = modelMatrix * windowMatrix;

                // Store the transformation matrix for the window
                windowTransformations.push_back(finalWindowMatrix);
            }
        }
    }

    // Getter methods for transformation matrices
    std::vector<glm::mat4> getMainCubeTransformations()
    {
        return mainCubeTransformations;
    }

    std::vector<glm::mat4> getWindowTransformations()
    {
        return windowTransformations;
    }
};