#ifndef _EXAMPLE_H
#define _EXAMPLE_H

#include <GL/gl.h>
#include <vector>

using std::vector;

struct Vertex {
    float x, y, z;
    Vertex(float x, float y, float z) 
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

struct Color {
    float r, g, b;
    Color(float r, float g, float b) 
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

class Example 
{
public:
    Example();

    bool init();
    void prepare(float dt);
    void render();
    void shutdown();

    void onResize(int width, int height);

private:
    void initializeCube();
    void renderCube();
    void updateProjection();

    enum Buffers {
        VERTEX_BUFFER,
        INDEX_BUFFER,
        COLOR_BUFFER,
        MAX_BUFFERS
    };

    GLuint m_vbos[MAX_BUFFERS];

    bool m_perspective;
    vector<GLuint> m_indices;
    vector<Vertex> m_vertices;
    vector<Color> m_colors;
};

#endif