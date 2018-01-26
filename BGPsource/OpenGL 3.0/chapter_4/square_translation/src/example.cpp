#ifdef _WIN32
#include <windows.h>
#else
#include <GL/glx.h>
#endif

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include "glext.h"
#include "example.h"

PFNGLGENBUFFERSARBPROC glGenBuffers = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;

Example::Example()
{
    m_zPosition = -10.0f;
    m_currentDirection = FORWARD;
}

bool Example::init()
{
#ifdef _WIN32
    glGenBuffers = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
#else
    glGenBuffers = (PFNGLGENBUFFERSARBPROC)glXGetProcAddress((const GLubyte*)"glGenBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC)glXGetProcAddress((const GLubyte*)"glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)glXGetProcAddress((const GLubyte*)"glBufferData");
#endif

    if (!glGenBuffers || !glBindBuffer || !glBufferData)
    {
        std::cerr << "VBOs are not supported by your graphics card" << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);

    GLfloat vertex [] = { -2.0f, -2.0f, -2.0f,
                           2.0f, -2.0f, -2.0f,
                          -2.0f, -2.0f, 2.0f,
                           2.0f, -2.0f, 2.0f };

    GLfloat colors [] = {   1.0f, 0.0f, 0.0f,
                            0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 1.0f,
                            1.0f, 1.0f, 0.0f };

    glGenBuffers(1, &m_vertexBuffer); //Generate a buffer for the vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, &vertex[0], GL_STATIC_DRAW); //Send the data to OpenGL

    glGenBuffers(1, &m_colorBuffer); //Generate a buffer for the vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, &colors[0], GL_STATIC_DRAW); //Send the data to OpenGL



    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    //Return success
    return true;
}

void Example::prepare(float dt)
{
    const float nearLimit = -10.0f;
    const float farLimit = -30.0f;
    const float speed = 15.0f;

    //If we are moving in the -z direction, decrement the position
    if (m_currentDirection == FORWARD) {
        m_zPosition -= speed * dt;
    } else { //otherwise we are moving backwards so increment the position
        m_zPosition += speed * dt;
    }

    //If we hit either the near or far limit, reverse the direction
    if (m_zPosition >= nearLimit) {
        m_currentDirection = FORWARD;
        m_zPosition = nearLimit;
    } else if (m_zPosition <= farLimit) {
        m_currentDirection = BACKWARD;
        m_zPosition = farLimit;
    }


}

void Example::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Load the identity matrix (reset to the default position and orientation)
    glLoadIdentity();

    //Bind the color array, and set the color pointer to point at it
    glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
    glColorPointer(3, GL_FLOAT, 0, 0);

    //Bind the vertex array and set the vertex pointer to point at it
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    //Translate using our zPosition variable
    glTranslatef(0.0, 0.0, m_zPosition);

    //Draw the square as a triangle strip
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Example::shutdown()
{

}

void Example::onResize(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(52.0f, float(width) / float(height), 1.0f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
