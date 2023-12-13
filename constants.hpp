/// @brief This file contains all the constants used in the program

#include <string>

using namespace std;

const string WINDOW_TITLE = "A simple simulation";
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

const float CAMERA_SPEED = 0.05f;
const float CAMERA_SPEED_FAST = 0.2f;
const float PITCH_CONSTRAINT = 89.0f;


// cube generation
const int NUM_CUBES_MIN = 1;
const int NUM_CUBES_MAX = 15;
const float CUBE_POS_MIN = -15.0f;
const float CUBE_POS_MAX = 15.0f;
const float CUBE_ROT_MIN = -180.0f;
const float CUBE_ROT_MAX = 180.0f;
const float CUBE_SCALE_MIN = 0.5f;
const float CUBE_SCALE_MAX = 2.0f;

// window generation
const int NUM_WINDOWSMIN = 1;
const int NUM_WINDOWSMAX = 5;

// Time of day
const float TIME_OF_DAY_INCREMENT = 0.002f;