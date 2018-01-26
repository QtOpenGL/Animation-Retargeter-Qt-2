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

    m_vertices.push_back(-0.5f); //X
    m_vertices.push_back(0.0f); //Y
    m_vertices.push_back(0.0f); //Z

    m_vertices.push_back(0.5f);
    m_vertices.push_back(0.0f);
    m_vertices.push_back(0.0f);

    m_vertices.push_back(-0.5f);
    m_vertices.push_back(0.0f);
    m_vertices.push_back(-1.0f);

	m_vertices.push_back(0.5f);
    m_vertices.push_back(0.0f);
    m_vertices.push_back(-1.0f);

    glGenBuffers(1, &m_vertexBuffer); //Generate a buffer for the vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_vertices.size(),
                 &m_vertices[0], GL_STATIC_DRAW); //Send the data to OpenGL

    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    m_rotation = 0.0f;

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    //Return success
    return true;
}

void Example::prepare(float dt)
{
    const float ROTATE_SPEED = 300.0f;

    m_rotation += ROTATE_SPEED * dt;

    if (m_rotation > 360.0f)
    {
        m_rotation -= 360.0f;
    }

}

void Example::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 5.0, 0.1, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPushMatrix();
        glTranslatef(-2.0f, 0.0f, 0.0f);
        glRotatef(m_rotation, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glPopMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    glPushMatrix();
        glTranslatef(-0.0f, 0.0f, 0.0f);
        glRotatef(m_rotation, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glPopMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
        glTranslatef(2.0f, 0.0f, 0.0f);
        glRotatef(m_rotation, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
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
