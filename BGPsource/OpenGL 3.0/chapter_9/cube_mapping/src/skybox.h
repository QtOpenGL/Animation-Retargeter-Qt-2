#ifndef BOGLGP_SKYBOX_H
#define BOGLGP_SKYBOX_H

#include <string>
#include "geom.h"
#include "targa.h"
#include "glee/GLee.h"

using std::string;

enum SKYBOX_SIDES {
    SS_NORTH = 0,
    SS_EAST,
    SS_SOUTH,
    SS_WEST,
    SS_TOP,
    SS_BOTTOM,
    SS_MAX_SIDES
};

class GLSLProgram;

class Skybox 
{
public:
    Skybox(const string& north,
           const string& east,
           const string& south,
           const string& west,
           const string& top,
           const string& bottom);

    bool initialize();
    void render(float x, float y, float z, GLSLProgram* program);
private:
    TargaImage m_images[SS_MAX_SIDES];
    GLuint m_texID[SS_MAX_SIDES];
    string m_texturePath[SS_MAX_SIDES];

    GLuint m_vertexBuffer;
    GLuint m_texCoordBuffer;

    vector<Vertex> m_vertices;
    vector<TexCoord> m_texCoords;
};

#endif