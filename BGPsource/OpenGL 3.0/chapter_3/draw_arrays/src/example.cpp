#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <GL/glu.h>
#include "example.h"

Example::Example()
{
}

bool Example::init()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    m_vertices.push_back(-2.0f); //X
    m_vertices.push_back(-2.0f); //Y
    m_vertices.push_back(0.0f);  //Z

    m_vertices.push_back(2.0f);  //X
    m_vertices.push_back(-2.0f); //Y
    m_vertices.push_back(0.0f);  //Z

    m_vertices.push_back(0.0f); //X
    m_vertices.push_back(2.0f); //etc..
    m_vertices.push_back(0.0f);

    //Return success
    return true;
}

void Example::prepare(float dt)
{

}

void Example::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -6.0f);

    glEnableClientState(GL_VERTEX_ARRAY); //Enable the vertex array
    glVertexPointer(3, GL_FLOAT, 0, &m_vertices[0]); //Tell OpenGL where the vertices are

    glDrawArrays(GL_TRIANGLES, 0, 3);

    //Finally disable the vertex array
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
