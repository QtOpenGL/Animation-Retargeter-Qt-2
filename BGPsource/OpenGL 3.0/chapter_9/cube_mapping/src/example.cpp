#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

#include "glee/GLee.h"
#include <GL/glu.h>

#include "example.h"
#include "glslshader.h"
#include "skybox.h"

Example::Example()
{
    m_basicProgram = new GLSLProgram("data/basic.vert", "data/basic.frag");
    m_cubeProgram = new GLSLProgram("data/cube.vert", "data/cube.frag");
    m_angle = 0.0f;
    m_cubeMapInitialized = false;

    m_skybox = new Skybox("data/skybox/bk.tga",
        "data/skybox/rt.tga",
        "data/skybox/ft.tga",
        "data/skybox/lt.tga",
        "data/skybox/up.tga",
        "data/skybox/dn.tga");
}

Example::~Example() 
{
    delete m_basicProgram;
    delete m_cubeProgram;
    delete m_skybox;
}

bool Example::init()
{
    if (!m_basicProgram->initialize() || !m_cubeProgram->initialize()) 
    {
        std::cerr << "Could not initialize the shaders" << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);

    m_cubeProgram->bindAttrib(0, "a_Vertex");
    m_cubeProgram->bindAttrib(1, "a_TexCoord0");
    m_cubeProgram->bindAttrib(2, "a_Normal");
    m_cubeProgram->linkProgram();

    //Bind the attribute locations
	m_basicProgram->bindAttrib(0, "a_Vertex");
    m_basicProgram->bindAttrib(1, "a_TexCoord0");

	//Re link the program
	m_basicProgram->linkProgram();	
    m_basicProgram->bindShader();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if (!m_rockyTexture.load("data/rocky.tga")) 
    {
        std::cerr << "Could not load rocky texture" << std::endl;
        return false;
    }

    glGenTextures(1, &m_rockyTexID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_rockyTexID);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, m_rockyTexture.getWidth(), 
                      m_rockyTexture.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, 
                      m_rockyTexture.getImageData());

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

    m_moonOne.initialize(10, 10, 0.2f, m_basicProgram);
    m_moonTwo.initialize(10, 10, 0.4f, m_basicProgram);
    m_moonThree.initialize(20, 20, 0.6f, m_basicProgram);

    m_planet.initialize(60, 60, 2.0f, m_cubeProgram);

    glGenTextures(1, &m_cubeMapTexID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapTexID);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    if (!m_skybox->initialize()) {
        return false;
    }

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

void Example::renderMovingObjects() 
{
    m_basicProgram->bindShader();
    m_basicProgram->sendUniform("texture0", 0);

    glBindTexture(GL_TEXTURE_2D, m_rockyTexID);
   
    glPushMatrix();
    glRotatef(m_angle + 270.0f, 0, 1, 0);
    glTranslatef(4.0, 0, 0);    
    m_moonOne.render();
    glPopMatrix();

    glPushMatrix();
    glRotatef(35.0f, 1, 0, 0);
    glRotatef(-m_angle*2, 0, 1, 0);
    glTranslatef(3.0, 0, 0);    
    m_moonTwo.render();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, m_iceTexID);
    glPushMatrix();
    glRotatef(m_angle*3, 0, 1, 0);
    glTranslatef(6.5, 0, 0);
    m_moonThree.render();
    glPopMatrix();
}

void Example::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Load the identity matrix (reset to the default position and orientation)
    glLoadIdentity();
    
    beginCubeMapRendering(256);
    for (int i = 0; i < 6; ++i)
    {
        //

        beginCubeMapFace(i);
        m_skybox->render(0, 0, 0, m_basicProgram);
        renderMovingObjects();
        finishCubeMapFace(i, 256);
    }
    endCubeMapRendering();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Load the identity matrix (reset to the default position and orientation)
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -12.0f);

    m_skybox->render(0, 0, 0, m_basicProgram);
    //m_skybox->render(0, 0, 0);

    m_cubeProgram->bindShader();
    m_cubeProgram->sendUniform("camera_position", 0.0f, 0.0f, 0.0f);
    m_cubeProgram->sendUniform("texture0", 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapTexID);
    m_planet.render();
    
    renderMovingObjects();   
}

void Example::shutdown()
{

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


void Example::beginCubeMapRendering(int texSize)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    m_viewportWidth = viewport[2];
    m_viewportHeight = viewport[3];

    glViewport(0, 0, texSize, texSize);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, 1, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void Example::beginCubeMapFace(int i) 
{
    const GLfloat ROTATION[6][6] = {
      // at              up
      { 1.0, 0.0, 0.0,   0.0, -1.0, 0.0 },  // pos x
      { -1.0, 0.0, 0.0,  0.0, -1.0, 0.0 },  // neg x
      { 0.0, 1.0, 0.0,   0.0, 0.0, 1.0 },   // pos y
      { 0.0, -1.0, 0.0,  0.0, 0.0, -1.0 },  // neg y
      { 0.0, 0.0, 1.0,   0.0, -1.0, 0.0 },  // poz z
      { 0.0, 0.0, -1.0,  0.0, -1.0, 0.0 }   // neg z
    };

    glClear(GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 0.0,
              ROTATION[i][0], ROTATION[i][1], ROTATION[i][2],
              ROTATION[i][3], ROTATION[i][4], ROTATION[i][5]);
}

void Example::finishCubeMapFace(int i, int texSize)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapTexID);
    if (m_cubeMapInitialized)
    {
        glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                            0, 0, 0, 0, 0, texSize, texSize);
    } else
    {
        glCopyTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB8, 0, 0, texSize, texSize, 0);
    }
}

void Example::endCubeMapRendering()
{
    m_cubeMapInitialized = true;
    onResize(m_viewportWidth, m_viewportHeight);
}