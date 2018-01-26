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

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

PFNGLGENBUFFERSARBPROC glGenBuffers = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;

Example::Example()
{
    m_xRotation = m_yRotation = 0.0f;
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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    GLfloat vertex [] = { -2.0f, 0.0f, -2.0f, //First Square vertex
                           2.0f, 0.0f, -2.0f,
                          -2.0f, 0.0f, 2.0f,
                           2.0f, 0.0f, 2.0f,  //Last Square vertex
                           0.0f, 0.0f, 0.0f,
                           0.0f, 3.0f, 0.0f,
                           0.0f, 0.0f, 0.0f,
                           3.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 0.0f,
                           0.0f, 3.0f, 0.0f,
                           0.0f, 0.0f, 0.0f,
                           3.0f, 0.0f, 0.0f
    };

    GLfloat colors [] = {   1.0f, 1.0f, 1.0f,
                            1.0f, 1.0f, 1.0f,
                            1.0f, 1.0f, 1.0f,
                            1.0f, 1.0f, 1.0f,
                            1.0f, 1.0f, 0.0f, //Start of yellow axis colors
                            1.0f, 1.0f, 0.0f,
                            1.0f, 1.0f, 0.0f,
                            1.0f, 1.0f, 0.0f, //End of yellow axis colors
                            1.0f, 1.0f, 1.0f, //Start of white axis colors
                            1.0f, 1.0f, 1.0f,
                            1.0f, 1.0f, 1.0f,
                            1.0f, 1.0f, 1.0f //End of white axis colors*/
    };

    glGenBuffers(1, &m_vertexBuffer); //Generate a buffer for the vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 36, &vertex[0], GL_STATIC_DRAW); //Send the data to OpenGL

    glGenBuffers(1, &m_colorBuffer); //Generate a buffer for the vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 36, &colors[0], GL_STATIC_DRAW); //Send the data to OpenGL



    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glLineWidth(2);
    //Return success
    return true;
}

void Example::prepare(float dt)
{
    const float xSpeed = 20.0f;
    const float ySpeed = 100.0f;

    m_xRotation += xSpeed * dt;
    m_yRotation += ySpeed * dt;

    if (m_xRotation > 360.0f) {
        m_xRotation -= 360.0f;
    }

    if (m_yRotation > 360.0f) {
        m_yRotation -= 360.0f;
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
    //glTranslatef(0.0, 0.0, m_zPosition);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    gluLookAt(0.0f, 5.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glDrawArrays(GL_LINES, 8, 4); //Render the white axis

    glPushMatrix();
        glRotatef(m_xRotation, 1.0f, 0.0f, 0.0f);
        glRotatef(m_yRotation, 0.0f, 1.0f, 0.0f);

        //Draw the square as a triangle strip
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_LINES, 4, 4); //Render the yellow axis
    glPopMatrix();


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
