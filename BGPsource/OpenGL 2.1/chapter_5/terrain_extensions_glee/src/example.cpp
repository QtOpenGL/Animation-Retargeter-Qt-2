#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include "glee/GLee.h"
#include <GL/glu.h>

#include "example.h"

Example::Example()
{

}

bool Example::init()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.5f, 0.9f, 0.5f);

    glEnableClientState(GL_VERTEX_ARRAY);

    if (!m_terrain.loadHeightmap("data/heightmap.raw", 65))
    {
        std::cerr << "Could not load the terrain" << std::endl;
        return false;
    }

    //Return success
    return true;
}



void Example::prepare(float dt)
{

}

void Example::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Load the identity matrix (reset to the default position and orientation)
    glLoadIdentity();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glTranslatef(0.0f, -20.0f, 0.0f);
    glRotatef(25.0f, 1.0f, 0.0f, 0.0f);
    //Translate using our zPosition variable
    glTranslatef(0.0, 0.0, -50.0f);

    m_terrain.render();
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
