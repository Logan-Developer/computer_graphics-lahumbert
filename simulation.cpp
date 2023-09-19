#include <cmath>
#include <iostream>
#include <fstream>

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <GLFW/glfw3.h>

#include "constants.h"

using namespace std;

void initWindow()
{
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowPosition(80, 80);
    glutInitWindowSize(SCREEN_X, SCREEN_Y);

    glutCreateWindow(WINDOW_TITLE.c_str());
}

void initGlew() {
    GLenum err=glewInit();
    if (err!=GLEW_OK) {
      cerr << "GLEW NOT OK:" << err << endl;
      cerr << "Message" << glewGetErrorString(err)<< endl;
    }
    if (!GLEW_VERSION_2_1) {
      cerr << "GLEW NOT 2.1 or better" <<endl;
    }
}

void display() {
    glClearColor(0.0, 0.0, 0.0, 0.0); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);     // Clear the color buffer (background)

    glFlush();  // Render now
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    initWindow();

    initGlew();

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}