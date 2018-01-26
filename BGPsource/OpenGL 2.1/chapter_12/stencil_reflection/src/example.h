#ifndef _EXAMPLE_H
#define _EXAMPLE_H

#ifdef WIN32
#include <windows.h>
#endif

#include <vector>
#include <string>

#include "targa.h"
#include "sphere.h"
#include "plane.h"

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
    GLSLProgram* m_basicProgram;

    Sphere m_sphere;
    Plane m_plane;

    TargaImage m_checkerboardTexture;
    unsigned int m_checkerboardTexID;

    TargaImage m_iceTexture;
    unsigned int m_iceTexID;


    float m_angle;
};

#endif
