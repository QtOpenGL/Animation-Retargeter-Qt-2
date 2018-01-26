#ifndef _EXAMPLE_H
#define _EXAMPLE_H

#include <vector>
#include <string>
#include <GL/gl.h>
#include <GL/glu.h>
#include "glext.h"
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
    vector<string> getSupportedExtensions();
    bool isExtensionSupported(const string& ext);

    Terrain m_terrain;
};

extern PFNGLGENBUFFERSARBPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;

#endif