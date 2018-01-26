#ifndef BOGLGP_SPHERE_H
#define BOGLGP_SPHERE_H
#include <vector>
using std::vector;

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

    unsigned int m_vertexBuffer;
    unsigned int m_texCoordBuffer;
    unsigned int m_normalBuffer;

    GLSLProgram* m_shaderPtr;

};

#endif
