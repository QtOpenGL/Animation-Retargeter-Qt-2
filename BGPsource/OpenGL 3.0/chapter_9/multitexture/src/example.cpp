#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <ctime>

#include "glee/GLee.h"
#include <GL/glu.h>

#include "example.h"
#include "glslshader.h"

Example::Example()
{
    m_GLSLProgram = new GLSLProgram("data/basic-fixed.vert", "data/basic-fixed.frag");
    m_waterProgram = new GLSLProgram("data/water.vert", "data/water.frag");
    m_rotationAngle = 0.0f;
}

Example::~Example() 
{
    delete m_GLSLProgram;
    delete m_waterProgram;
}

bool Example::init()
{
    if (!m_GLSLProgram->initialize() || !m_waterProgram->initialize()) 
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
    m_GLSLProgram->bindAttrib(1, "a_TexCoord0");
    m_GLSLProgram->bindAttrib(2, "a_Normal");
    m_GLSLProgram->bindAttrib(3, "a_TexCoord1");
	
    m_waterProgram->bindAttrib(0, "a_Vertex");
    m_waterProgram->bindAttrib(1, "a_TexCoord0");

	//Re link the program
	m_GLSLProgram->linkProgram();	
	m_GLSLProgram->bindShader(); //Enable our shader

    m_waterProgram->linkProgram();
    m_waterProgram->bindShader();



    if (!m_waterTexture.load("data/water.tga"))
    {
        std::cerr << "Could not load the water texture" << std::endl;
        return false;
    }



    glGenTextures(1, &m_waterTexID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_waterTexID);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, m_waterTexture.getWidth(), 
                      m_waterTexture.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, 
                      m_waterTexture.getImageData());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);

    if (!Tree::initialize())  
    {
        return false;
    }

    positionTrees(5.0f, 0.94f);

    //Return success
    return true;
}

void Example::positionTrees(float heightThreshold, float percentage)
{
    srand((unsigned) time(NULL));
    for (int z = 0; z < 65; ++z) 
    {
        for (int x = 0; x < 65; ++x)
        {
            Vertex v = m_terrain.getPositionAt(x, z);
            if (v.y > heightThreshold) 
            {
                if (rand()/((double)RAND_MAX + 1) > percentage) {                  
                    m_trees.push_back(Tree(v.x, v.y, v.z));
                }
            }
        }
    }
}


void Example::prepare(float dt)
{
    m_rotationAngle += 5.0f * dt;
    if (m_rotationAngle > 360.0f) {
        m_rotationAngle -= 360.0f;   
    }
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

void Example::render()
{
    float modelviewMatrix[16];
    float projectionMatrix[16];

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Load the identity matrix (reset to the default position and orientation)
    glLoadIdentity();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glTranslatef(0.0f, -12.0f, 0.0f);
    glRotatef(25.0f, 1.0f, 0.0f, 0.0f);
    //Translate using our zPosition variable
    glTranslatef(0.0, 0.0, -30.0f);
    glRotatef(m_rotationAngle, 0, 1, 0);

    //Get the current matrices from OpenGL
    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);
    vector<float> normalMatrix = calculateNormalMatrix(modelviewMatrix);

    m_GLSLProgram->bindShader();

    //Send the modelview and projection matrices to the shaders
    m_GLSLProgram->sendUniform4x4("modelview_matrix", modelviewMatrix);
    m_GLSLProgram->sendUniform4x4("projection_matrix", projectionMatrix);
    m_GLSLProgram->sendUniform3x3("normal_matrix", &normalMatrix[0]);
    
    m_GLSLProgram->sendUniform("texture0", 0);
    m_GLSLProgram->sendUniform("texture1", 1); //Set the uniform for the height texture

    //Send the lighting properties
    m_GLSLProgram->sendUniform("material_ambient", 0.2f, 0.2f, 0.2f, 1.0f);
    m_GLSLProgram->sendUniform("material_diffuse", 0.8f, 0.8f, 0.8f, 1.0f);
    m_GLSLProgram->sendUniform("material_specular", 0.6f, 0.6f, 0.6f, 1.0f);
    m_GLSLProgram->sendUniform("material_emissive", 0.0f, 0.0f, 0.0f, 1.0f);
    m_GLSLProgram->sendUniform("material_shininess", 10.0f);
    m_GLSLProgram->sendUniform("light0.ambient", 0.0f, 0.0f, 0.0f, 1.0f);
    m_GLSLProgram->sendUniform("light0.diffuse", 1.0f, 1.0f, 1.0f, 1.0f);
    m_GLSLProgram->sendUniform("light0.specular", 0.3f, 0.3f, 0.3f, 1.0f);
    m_GLSLProgram->sendUniform("light0.position", 0.0f, 0.4f, 1.0f, 0.0f);
    
    m_terrain.render();

    for (vector<Tree>::iterator tree = m_trees.begin(); tree != m_trees.end(); ++tree) 
    {
        (*tree).render();
    }

    m_waterProgram->bindShader();
    m_waterProgram->sendUniform4x4("modelview_matrix", modelviewMatrix);
    m_waterProgram->sendUniform4x4("projection_matrix", projectionMatrix);
    m_waterProgram->sendUniform3x3("normal_matrix", &normalMatrix[0]);
    glBindTexture(GL_TEXTURE_2D, m_waterTexID);
    m_terrain.renderWater();


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
