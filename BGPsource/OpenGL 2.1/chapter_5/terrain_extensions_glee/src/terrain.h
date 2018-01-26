#ifndef BOGLGP_TERRAIN_H
#define BOGLGP_TERRAIN_H

#ifdef _WIN32
#include <windows.h>
#endif

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

    unsigned int m_vertexBuffer;
    unsigned int m_indexBuffer;
    unsigned int m_colorBuffer;

    vector<Vertex> m_vertices;
    vector<unsigned int> m_indices;
    vector<Color> m_colors;
};

#endif
