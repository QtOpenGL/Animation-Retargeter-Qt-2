#include <cmath>
#include "sphere.h"
#include "glslshader.h"

Vertex* normalize(Vertex* in) 
{
    float l = sqrtf(in->x * in->x + in->y * in->y + in->z * in->z);
    in->x = in->x / l;
    in->y = in->y / l;
    in->z = in->z / l;
    return in;
}

bool Sphere::initialize(int slices, int stacks, float radius, GLSLProgram* ptr)
{
    m_shaderPtr = ptr;

    const float PI = 3.14159f;

    m_vertices.clear();

    m_stacks = stacks;
    m_slices = slices;


    float x = 0.0f, y = 0.0f, z = 0.0f, s = 0.0f, t = 0.0f;

    float PIOverStacks = PI / float(stacks);
    float PI2OverSlices = 2.0f * PI / float(slices);
    for (int stack = 0; stack < stacks; ++stack) 
    {
        unsigned size = m_vertices.size();

        float Phi = float(stack) * PIOverStacks;
        float CosP = cosf(Phi);
        float SinP = sinf(Phi);
        for (int slice = 0; slice < slices; ++slice)
        {
            float Theta = float(slice) * PI2OverSlices;
            x = radius * cosf(Theta) * SinP;
            y = radius * sinf(Theta) * SinP;
            z = radius * CosP;
            s = 1.0f - (float)slice / (float)slices;
            t = (float)stack / (float)stacks;
            
            m_texCoords.push_back(TexCoord(s, t));
            m_vertices.push_back(Vertex(x, y, z));
            Vertex n1(x, y, z);
            normalize(&n1);
            m_normals.push_back(n1);

            float nextPhi = float(stack + 1) * PIOverStacks;
            float nextCosP = cosf(nextPhi);
            float nextSinP = sinf(nextPhi);
            x = radius * cosf(Theta) * nextSinP;
            y = radius * sinf(Theta) * nextSinP;
            z = radius * nextCosP;
            s = 1.0f - (float)slice / (float)slices;
            t = (float)(stack + 1.0f) / (float)stacks;

            m_texCoords.push_back(TexCoord(s, t));
            m_vertices.push_back(Vertex(x, y, z));
            
            Vertex n2(x, y, z);
            normalize(&n2);
            m_normals.push_back(n2);
        }

        m_vertices.push_back(m_vertices[size]);
        m_texCoords.push_back(m_texCoords[size]);
        m_normals.push_back(m_normals[size]);

        m_vertices.push_back(m_vertices[size+1]);
        m_texCoords.push_back(m_texCoords[size+1]);
        m_normals.push_back(m_normals[size+1]);
        
    }

    glGenBuffers(1, &m_vertexBuffer); //Generate a buffer for the vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_vertices.size() * 3, &m_vertices[0], GL_STATIC_DRAW); //Send the data to OpenGL

    glGenBuffers(1, &m_texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer); //Bind the tex coord buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_texCoords.size() * 2, &m_texCoords[0], GL_STATIC_DRAW); //Send the data to OpenGL

    glGenBuffers(1, &m_normalBuffer); //Generate a buffer for the vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_normals.size() * 3, &m_normals[0], GL_STATIC_DRAW); //Send the data to OpenGL
    return true;
}

void Sphere::render()
{
	glEnable(GL_CULL_FACE);

    static float modelviewMatrix[16];
    static float projectionMatrix[16];

    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

    m_shaderPtr->sendUniform4x4("modelview_matrix", modelviewMatrix);
    m_shaderPtr->sendUniform4x4("projection_matrix", projectionMatrix);

    int verticesPerStrip = (m_slices + 1) * 2;
    int start = 0;

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);    
    glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
    glVertexAttribPointer((GLint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
    glVertexAttribPointer((GLint)2, 3, GL_FLOAT, GL_FALSE, 0, 0);
 
    for(int stack = 0; stack < m_stacks; ++stack)
    {
        glDrawArrays(GL_TRIANGLE_STRIP, start, (m_slices * 2) + 2);
        start += verticesPerStrip;
    }

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    
}
