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
    // Define an array to store the transformation matrices for each cube
    std::vector<glm::mat4> cubeTransformations;
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

            // Translate the cube to the current position
            modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));

            // Rotate the cube to a random angle between CUBE_ROT_MIN and CUBE_ROT_MAX
            modelMatrix = glm::rotate(modelMatrix, glm::radians(static_cast<float>(rand()) / RAND_MAX * (CUBE_ROT_MAX - CUBE_ROT_MIN) + CUBE_ROT_MIN), glm::vec3(0.0f, 1.0f, 0.0f));

            // Scale the cube to a random size between CURBE_SCALE_MIN and CUBE_SCALE_MAX
            modelMatrix = glm::scale(modelMatrix, glm::vec3(static_cast<float>(rand()) / RAND_MAX * (CUBE_SCALE_MAX - CUBE_SCALE_MIN) + CUBE_SCALE_MIN));

            // Store the transformation matrix
            cubeTransformations.push_back(modelMatrix);
        }
    }
    
    std::vector<glm::mat4> getCubeTransformations()
    {
        return cubeTransformations;
    }
};