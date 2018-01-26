#ifndef PLANE_H
#define PLANE_H

#include <vector>
#include "geom.h"

class GLSLProgram;

class Plane
{
public:
    bool initialize(GLSLProgram* ptr);
    void render();
private:
    std::vector<Vertex> m_vertices;
    std::vector<TexCoord> m_texCoords;

    unsigned int m_vertexBuffer;
    unsigned int m_texCoordBuffer;

    GLSLProgram* m_shaderPtr;
};

#endif // PLANE_H
