#ifndef _EXAMPLE_H
#define _EXAMPLE_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <vector>
#include <string>

#include "targa.h"
#include "sphere.h"


class GLSLProgram; 
class Skybox;

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
    GLSLProgram* m_cubeProgram;

    void initializeMovingObjects();
    void updateMovingObjects();
    void renderMovingObjects();

    Sphere m_moonOne;
    Sphere m_moonTwo;
    Sphere m_moonThree;
    Sphere m_planet;

    float m_moonPositions[3][3];

    TargaImage m_rockyTexture;
    GLuint m_rockyTexID;

    TargaImage m_iceTexture;
    GLuint m_iceTexID;

    float m_angle;

    bool m_cubeMapInitialized;
    GLuint m_cubeMapTexID;

    void beginCubeMapRendering(int texSize);
    void beginCubeMapFace(int i);
    void finishCubeMapFace(int i, int texSize);
    void endCubeMapRendering();

    int m_viewportWidth, m_viewportHeight;

    Skybox* m_skybox;
};

#endif
