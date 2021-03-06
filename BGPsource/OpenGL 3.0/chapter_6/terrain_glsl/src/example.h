#ifndef _EXAMPLE_H
#define _EXAMPLE_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <vector>
#include <string>
#include "terrain.h"

class GLSLProgram;

using std::vector;
using std::string;

class Example
{
public:
    Example();
    virtual ~Example();

    bool init();
    void prepare(float dt);
    void render();
    void shutdown();

    void onResize(int width, int height);

private:
    Terrain m_terrain;
    GLSLProgram* m_GLSLProgram;
};

#endif
