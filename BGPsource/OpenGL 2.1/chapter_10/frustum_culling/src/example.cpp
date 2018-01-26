#ifdef _WIN32
#include <windows.h>
#endif

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>

#include "glee/GLee.h"
#include <GL/glu.h>

#include "example.h"
#include "glslshader.h"

#ifdef _WIN32
#include "glwindow.h"
#else
#include "glxwindow.h"
#endif

Example::Example()
{
    m_terrainProgram = new GLSLProgram("data/basic-fixed.vert", "data/basic-fixed.frag");
    m_modelProgram = new GLSLProgram("data/model.vert", "data/model.frag");
    m_rotationAngle = 0.0f;
    m_frustumCullingEnabled = true;
}

Example::~Example()
{
    delete m_terrainProgram;
    delete m_modelProgram;
}

bool Example::init()
{
    if (!m_terrainProgram->initialize() || !m_modelProgram->initialize())
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

    //Scale the heights of the terrain between 0 and 1
    m_terrain.normalizeTerrain();

    //Bind the attribute locations
	m_terrainProgram->bindAttrib(0, "a_Vertex");
    m_terrainProgram->bindAttrib(1, "a_TexCoord0");
    m_terrainProgram->bindAttrib(2, "a_Normal");
    m_terrainProgram->bindAttrib(3, "a_TexCoord1");

	//Re link the program
	m_terrainProgram->linkProgram();
	m_terrainProgram->bindShader(); //Enable our shader

    m_modelProgram->bindAttrib(0, "a_Vertex");
    m_modelProgram->bindAttrib(1, "a_TexCoord0");

    m_modelProgram->linkProgram();

    if (!m_ogroTexture.load("data/Ogro/Ogrobase.tga")) {
        std::cerr << "Could not load the model texture" << std::endl;
        return false;
    }

    glGenTextures(1, &m_ogroTextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_ogroTextureID);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, m_ogroTexture.getWidth(),
                      m_ogroTexture.getHeight(), GL_RGB, GL_UNSIGNED_BYTE,
                      m_ogroTexture.getImageData());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);

    if(!m_model.load("data/Ogro/tris.md2"))
    {
        std::cerr << "Could not load the ogro model" << std::endl;
        return false;
    }
    m_model.setAnimation(Animation::IDLE);

    positionMonsters(150);

    //Return success
    return true;
}


void Example::positionMonsters(int numMonsters)
{
    for (int i = 0; i < numMonsters; ++i)
    {
        const int range = 50;
        float x = float(rand() % range) - float(range) / 2.0f;
        float z = float(rand() % range) - float(range) / 2.0f;

        float y = m_terrain.getHeightAt((float)x, (float)z);
        m_ogros.push_back(Monster(&m_model, (float)x, y + 0.35f, (float)z));
    }
}

void Example::prepare(float dt)
{
    m_model.update(dt);
    updateFPS(dt);

#ifdef _WIN32
    static int lastPress = GetTickCount();

    //If the spacebar was pressed, and >300 milliseconds has passed since the last press
    //then toggle the frustum culling
    if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && (GetTickCount() - lastPress) > 300)
    {
        m_frustumCullingEnabled = !m_frustumCullingEnabled;
        lastPress = GetTickCount();
    }

    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        m_rotationAngle -= 25.0f * dt;
    }

    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        m_rotationAngle += 25.0f * dt;
    }
#else
    if (m_window->keyPressed(XK_space))
    {
        m_frustumCullingEnabled = !m_frustumCullingEnabled;
    }

    if (m_window->keyHeldDown(XK_Left))
    {
        m_rotationAngle -= 25.0f * dt;
    }

    if (m_window->keyHeldDown(XK_Right))
    {
        m_rotationAngle += 25.0f * dt;
    }
#endif
}

/**
Returns an array of 3x3 floats representing a suitable normal
matrix. This returns the inverse transpose of the passed in matrix

If you are going to normalize your normal after multiplication with
this matrix then you can use the transpose(adjoint(M)) rather than
the transpose(inverse(M)). The adjoint is like the inverse but isnt
divided by the matrix determinate.
*/
vector<float> Example::calculateNormalMatrix(const float* modelviewMatrix)
{
    /*
        0   1   2
    0   0   3   6
    1   1   4   7
    2   2   5   8
    */

    //Grab the top 3x3 of the modelview matrix
    vector<float> M(3 * 3);
    M[0] = modelviewMatrix[0];
    M[1] = modelviewMatrix[1];
    M[2] = modelviewMatrix[2];
    M[3] = modelviewMatrix[4];
    M[4] = modelviewMatrix[5];
    M[5] = modelviewMatrix[6];
    M[6] = modelviewMatrix[8];
    M[7] = modelviewMatrix[9];
    M[8] = modelviewMatrix[10];

    //Work out the determinate
    float determinate = M[0] * M[4] * M[8] + M[1] * M[5] * M[6] + M[2] * M[3] * M[7];
    determinate -= M[2] * M[4] * M[6] + M[0] * M[5] * M[7] + M[1] * M[3] * M[8];

    //One division is faster than several
    float oneOverDet = 1.0f / determinate;

    vector<float> N(3 * 3);

    //Calculate the inverse and assign it to the transpose matrix positions
    N[0] = (M[4] * M[8] - M[5] * M[7]) * oneOverDet;
    N[3] = (M[2] * M[7] - M[1] * M[8]) * oneOverDet;
    N[6] = (M[1] * M[5] - M[2] * M[4]) * oneOverDet;

    N[1] = (M[5] * M[6] - M[3] * M[8]) * oneOverDet;
    N[4] = (M[0] * M[8] - M[2] * M[6]) * oneOverDet;
    N[7] = (M[2] * M[3] - M[0] * M[5]) * oneOverDet;

    N[2] = (M[3] * M[7] - M[4] * M[6]) * oneOverDet;
    N[5] = (M[1] * M[6] - M[8] * M[7]) * oneOverDet;
    N[8] = (M[0] * M[4] - M[1] * M[3]) * oneOverDet;

    return N;
}

void Example::updateFPS(float dt)
{
    static int frame = 0;
    static float fps = 0.0f;

    const float FPS_UPDATE_INTERVAL = 1.0;

    fps += dt;
    ++frame;
    if (fps > FPS_UPDATE_INTERVAL)
    {
        m_FPS = float(frame) / fps;
        frame = 0;
        fps -= FPS_UPDATE_INTERVAL;
    }
}

void Example::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Load the identity matrix (reset to the default position and orientation)
    glLoadIdentity();

    glRotatef(m_rotationAngle, 0, 1, 0);
    glTranslatef(0.0f, -1.0f, 0.0f);

    m_frustum.updateFrustum();

    m_terrain.render(m_terrainProgram);

    glBindTexture(GL_TEXTURE_2D, m_ogroTextureID);

    int monstersRendered = 0;
    for(vector<Monster>::iterator i = m_ogros.begin(); i != m_ogros.end(); ++i) {
        float x, y, z;
        (*i).getXYZ(x, y, z); //Get the position of the monster

        //Check that it is inside the frustum
        if (m_frustumCullingEnabled)
        {
            if (m_frustum.sphereInFrustum(x, y, z, m_model.getRadius()))
            {
                (*i).render(m_modelProgram);
                monstersRendered++;
            }
        }
        else
        {
            (*i).render(m_modelProgram);
            monstersRendered++;
        }
    }

    std::stringstream ss;
    ss << "Chapter 10 - Frustum Culling. Monsters rendered: " << monstersRendered;
    ss << " - Culling enabled: " << ((m_frustumCullingEnabled)? "Yes" : "No");
    ss << " - FPS: " << std::setprecision(3) << m_FPS;

#ifdef _WIN32
    m_window->setWindowCaption(ss.str());
#else
	m_window->setWindowCaption(ss.str());
#endif
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
