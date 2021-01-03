#include "UIText.h"
//#include <Windows.h>
//#include <glew.h>
//#include <freeglut.h>
//#include <iostream>

// Code found from https://users.cs.jmu.edu/bernstdh/web/common/lectures/summary_opengl-text.php ---------------------------------------------

/*
* Draw a character string.
*
* @param x        The x position
* @param y        The y position
* @param z        The z position
* @param string   The character string

void drawString(float x, float y, float z, char* string) {
    glRasterPos3f(x, y, z);

    for (char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(currentFont, *c);  // Updates the position
    }
}


/**
* The reshape callback.
*
* @param w   The width
* @param h   The height

void reshape(int w, int h) {
    GLfloat aspect;

    // Save the width and height for text processing
    width = w;
    height = h;

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    if (w <= h) {
        aspect = (GLfloat)h / (GLfloat)w;
        glOrtho(-1.5, 1.5, -1.5 * aspect, 1.5 * aspect, -10.0, 10.0);
    }
    else {
        aspect = (GLfloat)w / (GLfloat)h;
        glOrtho(-1.5 * aspect, 1.5 * aspect, -1.5, 1.5, -10.0, 10.0);
    }
}

/**
 * The entry point of the application.
 *
 * This function contains calls to GLUT.
 *
 * @param argc  The number of command line arguments
 * @param argv  The array of command line arguments
 * @return      A status code
 
int main(int argc, char** argv) {
    glutInit(&argc, argv);

    window = createWindow("Bitmap Fonts", 640, 480, 0, 0);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    init();

    glutMainLoop();
}

/**
* Display/render the OpenGL window.

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);

    // Set things up so that we can render text in
    // window coordinates
    beginText();

    // Render the text
    drawString(0.0, 0.0, "James Madison University");

    // Set things up for normal rendering
    endText();

    // Normal rendering of the scene goes here

    glFlush();
}

/**
* Setup the state for text.
void beginText() {
    glMatrixMode(GL_PROJECTION);

    // Save the current projection matrix
    glPushMatrix();

    // Make the current matrix the identity matrix
    glLoadIdentity();

    // Set the projection (to 2D orthographic)
    gluOrtho2D(0, width, 0, height);

    glMatrixMode(GL_MODELVIEW);
}

/**
* End the state for text.

void endText() {
    glMatrixMode(GL_PROJECTION);

    // Restore the original projection matrix
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

Fonts:
GLUT_STROKE_ROMAN
GLUT_STROKE_MONO_ROMAN
These are both void*
Functions :
    void glutStrokeCharacter(void* font, char* c)
    int glutStrokeWidth(void* font, char* c)

    /**
* Draw a character string.
*
* @param x        The x position
* @param y        The y position
* @param z        The z position
* @param string   The character string

void drawString(float x, float y, float z, char* string) {
    // Save the current matrix
    glPushMatrix();

    // Translate to the appropriate starting point
    glTranslatef(x, y, z);

    // Note: We could change the line width with glLineWidth()

    // Render the characters
    for (char* c = string; *c != '\0'; c++) {
        glutStrokeCharacter(currentFont, *c);
    }

    // Another useful function
    //    int glutStrokeWidth(void *font, int character);

    // Retrieve the original matrix
    glPopMatrix();
}*/
