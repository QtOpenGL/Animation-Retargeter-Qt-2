#ifndef _EXAMPLE_H
#define _EXAMPLE_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <vector>
#include <string>
#include "terrain.h"

using std::vector;
using std::string;

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
    Terrain m_terrain;
};

#endif
