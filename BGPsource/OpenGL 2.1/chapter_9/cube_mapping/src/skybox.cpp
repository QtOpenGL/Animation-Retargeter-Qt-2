
#include "skybox.h"
#include "glslshader.h"

#include <GL/glu.h>

Skybox::Skybox(const string& north,
           const string& east,
           const string& south,
           const string& west,
           const string& top,
           const string& bottom)
{
    m_texturePath[SS_NORTH] = north;
    m_texturePath[SS_EAST] = east;    
    m_texturePath[SS_SOUTH] = south;
    m_texturePath[SS_WEST] = west;
    m_texturePath[SS_TOP] = top;
    m_texturePath[SS_BOTTOM] = bottom;
}

bool Skybox::initialize() 
{
    glGenTextures(SS_MAX_SIDES, m_texID);

    for (int side = 0; side < SS_MAX_SIDES; ++side)
    {
        if(!m_images[side].load(m_texturePath[side]))
        {
            return false;
        }
	    
        glBindTexture(GL_TEXTURE_2D, m_texID[side]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, m_images[side].getWidth(), 
            m_images[side].getHeight(), GL_RGB, GL_UNSIGNED_BYTE, m_images[side].getImageData());
    }

    float SIZE = 75.0f;

	//Back
    m_vertices.push_back(Vertex(-SIZE,-SIZE,-SIZE));
    m_vertices.push_back(Vertex( SIZE,-SIZE,-SIZE));
    m_vertices.push_back(Vertex( SIZE, SIZE,-SIZE));
    m_vertices.push_back(Vertex(-SIZE, SIZE,-SIZE));

    m_texCoords.push_back(TexCoord(0.0f, 0.0f));
    m_texCoords.push_back(TexCoord(1.0f, 0.0f));
    m_texCoords.push_back(TexCoord(1.0f, 1.0f));
    m_texCoords.push_back(TexCoord(0.0f, 1.0f));

	//Left
    m_vertices.push_back(Vertex(-SIZE,-SIZE,-SIZE));
    m_vertices.push_back(Vertex(-SIZE, SIZE,-SIZE));
    m_vertices.push_back(Vertex(-SIZE, SIZE, SIZE));
    m_vertices.push_back(Vertex(-SIZE,-SIZE, SIZE));

    m_texCoords.push_back(TexCoord(1.0f, 0.0f));
    m_texCoords.push_back(TexCoord(1.0f, 1.0f));
    m_texCoords.push_back(TexCoord(0.0f, 1.0f));
    m_texCoords.push_back(TexCoord(0.0f, 0.0f));

	//Front
    m_vertices.push_back(Vertex( SIZE,-SIZE, SIZE));
    m_vertices.push_back(Vertex(-SIZE,-SIZE, SIZE));
    m_vertices.push_back(Vertex(-SIZE, SIZE, SIZE));
    m_vertices.push_back(Vertex( SIZE, SIZE, SIZE));

    m_texCoords.push_back(TexCoord(0.0f, 0.0f));
    m_texCoords.push_back(TexCoord(1.0f, 0.0f));
    m_texCoords.push_back(TexCoord(1.0f, 1.0f));
    m_texCoords.push_back(TexCoord(0.0f, 1.0f));

	//Right
    m_vertices.push_back(Vertex( SIZE,-SIZE, SIZE));
    m_vertices.push_back(Vertex( SIZE, SIZE, SIZE));
    m_vertices.push_back(Vertex( SIZE, SIZE,-SIZE));
    m_vertices.push_back(Vertex( SIZE,-SIZE,-SIZE));

    m_texCoords.push_back(TexCoord(1.0f, 0.0f));
    m_texCoords.push_back(TexCoord(1.0f, 1.0f));
    m_texCoords.push_back(TexCoord(0.0f, 1.0f));
    m_texCoords.push_back(TexCoord(0.0f, 0.0f));

	//Top
    m_vertices.push_back(Vertex(-SIZE, SIZE, -SIZE));
    m_vertices.push_back(Vertex( SIZE, SIZE, -SIZE));
    m_vertices.push_back(Vertex( SIZE, SIZE,  SIZE));
    m_vertices.push_back(Vertex(-SIZE, SIZE,  SIZE));

    m_texCoords.push_back(TexCoord(0.0f, 0.0f));
    m_texCoords.push_back(TexCoord(1.0f, 0.0f));
    m_texCoords.push_back(TexCoord(1.0f, 1.0f));
    m_texCoords.push_back(TexCoord(0.0f, 1.0f));

	//Bottom
    m_vertices.push_back(Vertex( SIZE,-SIZE, -SIZE));
    m_vertices.push_back(Vertex(-SIZE,-SIZE, -SIZE));
    m_vertices.push_back(Vertex(-SIZE,-SIZE,  SIZE));
    m_vertices.push_back(Vertex( SIZE,-SIZE,  SIZE));

    m_texCoords.push_back(TexCoord(0.0f, 0.0f));
    m_texCoords.push_back(TexCoord(1.0f, 0.0f));
    m_texCoords.push_back(TexCoord(1.0f, 1.0f));
    m_texCoords.push_back(TexCoord(0.0f, 1.0f));

    glGenBuffers(1, &m_vertexBuffer); //Generate a buffer for the vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_vertices.size() * 3, &m_vertices[0], GL_STATIC_DRAW); //Send the data to OpenGL

    glGenBuffers(1, &m_texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer); //Bind the tex coord buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_texCoords.size() * 2, &m_texCoords[0], GL_STATIC_DRAW); //Send the data to OpenGL
    return true;
}

void Skybox::render(float x, float y, float z, GLSLProgram* program)
{
    glPushMatrix();
    glTranslatef(x, y, z);

    static float modelviewMatrix[16];
    static float projectionMatrix[16];

    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

    program->sendUniform4x4("modelview_matrix", modelviewMatrix);
    program->sendUniform4x4("projection_matrix", projectionMatrix);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);    
    glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
    glVertexAttribPointer((GLint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    for (int i = 0; i < 6; ++i)
    {
        glBindTexture(GL_TEXTURE_2D, m_texID[i]);
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
}