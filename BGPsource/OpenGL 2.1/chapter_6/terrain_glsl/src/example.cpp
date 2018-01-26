#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include "glee/GLee.h"
#include <GL/glu.h>

#include "example.h"
#include "glslshader.h"

Example::Example()
{
    m_GLSLProgram = new GLSLProgram("data/basic-fixed.vert", "data/basic-fixed.frag");
}

Example::~Example() 
{
    delete m_GLSLProgram;
}

bool Example::init()
{
    if (!m_GLSLProgram->initialize()) 
    {
        std::cerr << "Could not initialize the shaders" << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.5f, 0.9f, 0.5f);

    if (!m_terrain.loadHeightmap("data/heightmap.raw", 65)) 
    {
        std::cerr << "Could not load the terrain" << std::endl;
        return false;
    }

    //Bind the attribute locations
	m_GLSLProgram->bindAttrib(0, "a_Vertex");
	m_GLSLProgram->bindAttrib(1, "a_Color");
	
	//Re link the program
	m_GLSLProgram->linkProgram();
	
	m_GLSLProgram->bindShader(); //Enable our shader
    //Return success
    return true;
}



void Example::prepare(float dt)
{

}

void Example::render()
{
    float modelviewMatrix[16];
    float projectionMatrix[16];

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Load the identity matrix (reset to the default position and orientation)
    glLoadIdentity();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glTranslatef(0.0f, -20.0f, 0.0f);
    glRotatef(25.0f, 1.0f, 0.0f, 0.0f);
    //Translate using our zPosition variable
    glTranslatef(0.0, 0.0, -50.0f);

    //Get the current matrices from OpenGL
    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

    //Send the modelview and projection matrices to the shaders
    m_GLSLProgram->sendUniform("modelview_matrix", modelviewMatrix);
    m_GLSLProgram->sendUniform("projection_matrix", projectionMatrix);

    glEnableVertexAttribArray(0); //Enable the vertex attribute
    glEnableVertexAttribArray(1); //Enable the color attribute

    m_terrain.render();

	glDisableVertexAttribArray(0); //Disable the vertex attribute
    glDisableVertexAttribArray(1); //Disable the color attribute
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
