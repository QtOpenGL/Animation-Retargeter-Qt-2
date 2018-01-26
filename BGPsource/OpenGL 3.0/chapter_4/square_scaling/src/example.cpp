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
    m_scale = 1.0f;
    m_increasing = true;
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

    GLfloat vertex [] = { -2.0f, -2.0f, 0.0f,
                           2.0f, -2.0f, 0.0f,
                          -2.0f, 2.0f, 0.0f,
                           2.0f, 2.0f, 0.0f };

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
    const float minLimit = 0.1f;
    const float maxLimit = 2.0f;
    const float speed = 1.0f;

    //If we are increasing the scale then increment the scale variable
    if (m_increasing) {
        m_scale += speed * dt;
    } else { //otherwise decrement
        m_scale -= speed * dt;
    }

    //If we hit either the min or max limit, reverse the scaling direction
    if (m_scale >= maxLimit) {
        m_increasing = false;
    } else if (m_scale <= minLimit) {
        m_increasing = true;
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

    glTranslatef(0.0f, 0.0f, -10.0f); //Move 10 units back
    glScalef(m_scale, m_scale, m_scale); //Scale using our scaling variable
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
