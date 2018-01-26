#ifndef BOGLGP_TERRAIN_H
#define BOGLGP_TERRAIN_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <string>
#include <vector>
#include "glee/GLee.h"
#include "targa.h"
#include "geom.h"

using std::string;
using std::vector;


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

class GLSLProgram;

class Terrain 
{
public:
    Terrain();
    bool loadHeightmap(const string& rawFile, int width);
    void render(GLSLProgram* glslProgram);
    void renderWater(GLSLProgram* glslProgram);   
    void normalizeTerrain();

    Vertex getPositionAt(int x, int z);

    GLfloat getHeightAt(GLfloat x, GLfloat z)
    {
      float scaledX = x + (float)m_width / 2;
      float scaledZ = z + (float)m_width / 2;

      int x0 = (int)scaledX;
      if (scaledX < 0)
        --x0;
      int z0 = (int)scaledZ;
      if (scaledZ < 0)
        --z0;
      int p0, p1, p2, p3;

      p0 = (z0 * m_width + x0) + 1;
      p1 = (z0 * m_width + x0 + 1) + 1;
      p2 = ((z0 + 1) * m_width + x0) + 1;
      p3 = ((z0 + 1) * m_width + x0 + 1) + 1;

      float fracX = scaledX - x0;
      float fracZ = scaledZ - z0;

      float xInterp0 = m_vertices[p0].y + fracX * (m_vertices[p1].y - m_vertices[p0].y);
      float xInterp1 = m_vertices[p2].y + fracX * (m_vertices[p3].y - m_vertices[p2].y);

      return xInterp0 + fracZ * (xInterp1 - xInterp0);
    }


private:
    void generateVertices(const vector<float> heights, int width);
    void generateIndices(int width);
    void generateTexCoords(int width);
    void generateNormals();
    
    void generateWaterVertices(int width);
    void generateWaterIndices(int width);
    void generateWaterTexCoords(int width);

    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;
    GLuint m_colorBuffer;
    GLuint m_texCoordBuffer;
    GLuint m_heightTexCoordBuffer;

    GLuint m_normalBuffer;

    GLuint m_waterVertexBuffer;
    GLuint m_waterIndexBuffer;
    GLuint m_waterTexCoordsBuffer;

    vector<Vertex> m_vertices;
    vector<Color> m_colors;
    vector<TexCoord> m_texCoords;
    vector<float> m_heightTexCoords;

    vector<GLuint> m_indices;
    vector<Vertex> m_normals;

    vector<Vertex> m_waterVertices;
    vector<GLuint> m_waterIndices;
    vector<TexCoord> m_waterTexCoords;

    TargaImage m_grassTexture;
    TargaImage m_heightTexture;
    GLuint m_grassTexID;
    GLuint m_heightTexID;

    bool m_isMultitextureEnabled;
    int m_width;
};

#endif
