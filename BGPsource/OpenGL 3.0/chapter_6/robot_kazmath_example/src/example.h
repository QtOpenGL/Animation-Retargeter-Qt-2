#ifndef _EXAMPLE_H
#define _EXAMPLE_H

#include <vector>

#include <kazmath/GL/matrix.h>
#include <kazmath/mat4.h>

#include "Robot.h"

using std::vector;
class GLSLProgram;

class Example
{
public:
    Example();
    ~Example();

    bool init();
    void prepare(float dt);
    void render();
    void shutdown();

    void onResize(int width, int height);

private:
    Robot m_robot;
    float m_rotationAngle;
    GLSLProgram* m_shader;
};

#endif
