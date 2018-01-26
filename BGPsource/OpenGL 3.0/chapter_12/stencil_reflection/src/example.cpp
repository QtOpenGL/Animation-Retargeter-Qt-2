#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

#include "glee/GLee.h"

#include "example.h"
#include "glslshader.h"

Example::Example()
{
    m_basicProgram = new GLSLProgram("data/basic.vert", "data/basic.frag");
    m_angle = 0.0f;
}

Example::~Example()
{
    delete m_basicProgram;
}

bool Example::init()
{
    if (!m_basicProgram->initialize())
    {
        std::cerr << "Could not initialize the shaders" << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);

    //Bind the attribute locations
    m_basicProgram->bindAttrib(0, "a_Vertex");
    m_basicProgram->bindAttrib(1, "a_TexCoord0");

    //Re link the program
    m_basicProgram->linkProgram();
    m_basicProgram->bindShader();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    if (!m_checkerboardTexture.load("data/checkerboard.tga"))
    {
        std::cerr << "Could not load checkerboard texture" << std::endl;
        return false;
    }

    glGenTextures(1, &m_checkerboardTexID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_checkerboardTexID);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, m_checkerboardTexture.getWidth(),
                      m_checkerboardTexture.getHeight(), GL_RGB, GL_UNSIGNED_BYTE,
                      m_checkerboardTexture.getImageData());

    if (!m_iceTexture.load("data/ice.tga"))
    {
        std::cerr << "Could not load ice texture" << std::endl;
        return false;
    }

    glGenTextures(1, &m_iceTexID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_iceTexID);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, m_iceTexture.getWidth(),
                      m_iceTexture.getHeight(), GL_RGB, GL_UNSIGNED_BYTE,
                      m_iceTexture.getImageData());

    m_sphere.initialize(20, 20, 2.0f, m_basicProgram);
    m_plane.initialize(m_basicProgram);

    //Return success
    return true;
}


void Example::prepare(float dt)
{
    m_angle += 25.0f * dt;
    if (m_angle > 360.0f)
    {
        m_angle -= 360.0f;
    }

}

void Example::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);//we're clearing the stencil buffer too!
    //Load the identity matrix (reset to the default position and orientation)
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -20.0f);
    glRotatef(15.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(m_angle, 0.0f, 1.0f, 0.0f);


    m_basicProgram->sendUniform("texture0",0);

    //*************************************************************
    //Draw the plane with stencil on
    //*************************************************************

    glEnable(GL_STENCIL_TEST); //Enable stenciling
    glDepthMask(GL_FALSE); //Disable depth writes

    //Always replace the stencil value whether it passes or fails
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

    //Make the test always pass
    glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);


    glBindTexture(GL_TEXTURE_2D, m_checkerboardTexID);
    glPushMatrix();
    glScalef(15.0f, 1.0f, 15.0f);
    m_plane.render();
    glPopMatrix();

    //Re-enable depth writes
    glDepthMask(GL_TRUE);


    //*************************************************************
    //Draw the sphere's reflection, with blending and stencil testing
    //*************************************************************
    glBindTexture(GL_TEXTURE_2D, m_iceTexID);

    //Only render where the stencil value is 1
    glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
    //Don’t change the stencil values any more
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);

    glPushMatrix();
    glScalef(1.0f, -1.0f, 1.0f); //invert on the y axis -> we're reflecting!
	glCullFace(GL_FRONT);
    glTranslatef(0.0f, 2.0f, 0.0f);//move the sphere somewhat above the plane
    m_sphere.render();
	glCullFace(GL_BACK);
    glPopMatrix();

    glDisable(GL_BLEND);

    //Disable the stencil test
    glDisable(GL_STENCIL_TEST);

    //*************************************************************
    //Draw the sphere normal
    //*************************************************************

    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);//move the sphere somewhat above the plane
    m_sphere.render();
    glPopMatrix();

}

void Example::shutdown()
{
    glDeleteTextures(1,&m_checkerboardTexID);
    glDeleteTextures(1,&m_iceTexID);
}

void Example::onResize(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(52.0f, float(width) / float(height), 1.0f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
