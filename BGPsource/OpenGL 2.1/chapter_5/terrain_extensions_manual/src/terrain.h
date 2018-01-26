#ifndef BOGLGP_TERRAIN_H
#define BOGLGP_TERRAIN_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

struct Vertex 
{
    float x, y, z;
    Vertex(float x, float y, float z) 
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

struct Color 
{
    float r, g, b;
    Color(float r, float g, float b) 
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

class Terrain 
{
public:
    Terrain();
    bool loadHeightmap(const string& rawFile, int width);
    void render();

private:
    void generateVertices(const vector<float> heights, int width);
    void generateIndices(int width);

    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;
    GLuint m_colorBuffer;
 
    vector<Vertex> m_vertices;
    vector<GLuint> m_indices;
    vector<Color> m_colors;
};

#endif