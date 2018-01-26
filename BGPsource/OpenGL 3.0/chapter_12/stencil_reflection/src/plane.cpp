#include "plane.h"
#include "glee/GLee.h"
#include "glslshader.h"

bool Plane::initialize(GLSLProgram* ptr)
{
    m_shaderPtr = ptr;

    //Create a plane with size 1.0 x 1.0 lying on the xy-plane
    m_vertices.push_back(Vertex(-0.5,0.0,-0.5));
    m_vertices.push_back(Vertex(-0.5,0.0,0.5));
    m_vertices.push_back(Vertex(0.5,0.0,-0.5));
    m_vertices.push_back(Vertex(0.5,0.0,0.5));

    m_texCoords.push_back(TexCoord(0.0,0.0));
    m_texCoords.push_back(TexCoord(1.0,0.0));
    m_texCoords.push_back(TexCoord(0.0,1.0));
    m_texCoords.push_back(TexCoord(1.0,1.0));

    glGenBuffers(1, &m_vertexBuffer); //Generate a buffer for the vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_vertices.size() * 3, &m_vertices[0], GL_STATIC_DRAW); //Send the data to OpenGL

    glGenBuffers(1, &m_texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer); //Bind the tex coord buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_texCoords.size() * 2, &m_texCoords[0], GL_STATIC_DRAW); //Send the data to OpenGL

    return true;
}

void Plane::render()
{
    static float modelviewMatrix[16];
    static float projectionMatrix[16];

    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

    m_shaderPtr->sendUniform4x4("modelview_matrix", modelviewMatrix);
    m_shaderPtr->sendUniform4x4("projection_matrix", projectionMatrix);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
    glVertexAttribPointer((GLint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0,4);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}


