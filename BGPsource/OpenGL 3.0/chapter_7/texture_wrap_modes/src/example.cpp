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
    m_wrapMode = GL_REPEAT;
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
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);

    //Bind the attribute locations
	m_GLSLProgram->bindAttrib(0, "a_Vertex");
    m_GLSLProgram->bindAttrib(1, "a_TexCoord");
	
	//Re link the program
	m_GLSLProgram->linkProgram();
	
	m_GLSLProgram->bindShader(); //Enable our shader

    GLfloat vertex [] = { -2.0f, -2.0f, 0.0f, //First Square vertex
                           2.0f, -2.0f, 0.0f,
                           2.0f, 2.0f, 0.0f,
                          -2.0f, 2.0f, 0.0f  //Last Square vertex
    };

    GLfloat texCoords [] = { 0.0f, 0.0f, 
                             2.0f, 0.0f,
                             2.0f, 2.0f,
                             0.0f, 2.0f 
    };

    glGenBuffers(1, &m_vertexBuffer); //Generate a buffer for the vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, &vertex[0], GL_STATIC_DRAW); //Send the data to OpenGL

    glGenBuffers(1, &m_texCoordBuffer); //Generate a buffer for the vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, &texCoords[0], GL_STATIC_DRAW); //Send the data to OpenGL

    if (!m_texture.load("data/rock.tga"))
    {
        std::cerr << "Could not load the grass texture" << std::endl;
        return false;
    }

    glGenTextures(1, &m_texID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,0, GL_RGB8, m_texture.getWidth(), 
                      m_texture.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, 
                      m_texture.getImageData());

    glEnableVertexAttribArray(0); //Enable the vertex attribute
    glEnableVertexAttribArray(1); //Enable the color attribute


    //Return success
    return true;
}

void Example::prepare(float dt)
{
#ifdef WIN32
    static int lastPress = GetTickCount();
    if (GetAsyncKeyState(VK_SPACE) & 0x8000 && (GetTickCount() - lastPress) > 300)
    {       
        if (m_wrapMode == GL_REPEAT)
        {
            m_wrapMode = GL_CLAMP_TO_EDGE;
        }
        else if (m_wrapMode == GL_CLAMP_TO_EDGE)
        {
            m_wrapMode = GL_MIRRORED_REPEAT;
        }
        else if (m_wrapMode == GL_MIRRORED_REPEAT)
        {
            m_wrapMode = GL_REPEAT;
        }
        lastPress = GetTickCount();
        
        //Set the new wrap mode
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapMode);
    }
#endif
}

void Example::render()
{
    float modelviewMatrix[16];
    float projectionMatrix[16];

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Load the identity matrix (reset to the default position and orientation)
    glLoadIdentity();

    //Translate using our zPosition variable
    glTranslatef(0.0, 0.0, -5.0f);

    //Get the current matrices from OpenGL
    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

    //Send the modelview and projection matrices to the shaders
    m_GLSLProgram->sendUniform4x4("modelview_matrix", modelviewMatrix);
    m_GLSLProgram->sendUniform4x4("projection_matrix", projectionMatrix);
    m_GLSLProgram->sendUniform("texture0", 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
    glVertexAttribPointer((GLint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_QUADS, 0, 4);
    //glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

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
