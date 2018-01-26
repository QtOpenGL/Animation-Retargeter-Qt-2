#ifndef BOGLGP_SPHERE_H
#define BOGLGP_SPHERE_H
#include <vector>
using std::vector;

#include "glee/GLee.h"
#include "geom.h"

class GLSLProgram;

class Sphere 
{
public:
    bool initialize(int slices, int stacks, float radius, GLSLProgram* ptr);
    void render();
private:
    vector<Vertex> m_vertices;
    vector<TexCoord> m_texCoords;
    vector<Vertex> m_normals;

    int m_stacks;
    int m_slices;

    GLuint m_vertexBuffer;
    GLuint m_texCoordBuffer;
    GLuint m_normalBuffer;

    GLSLProgram* m_shaderPtr;

};

#endif