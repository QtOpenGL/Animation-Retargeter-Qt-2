#ifndef _EXAMPLE_H
#define _EXAMPLE_H

#include <GL/gl.h>
#include <vector>

using std::vector;

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
    GLuint m_vertexBuffer;
    GLuint m_colorBuffer;
    float m_zPosition;

    enum Direction {
        FORWARD,
        BACKWARD
    };

    Direction m_currentDirection;
};

#endif