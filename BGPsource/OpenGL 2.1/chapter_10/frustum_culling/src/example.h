#ifndef _EXAMPLE_H
#define _EXAMPLE_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <vector>
#include <string>
#include "md2model.h"
#include "terrain.h"
#include "targa.h"

#include "tree.h"
#include "monster.h"
#include "frustum.h"

class GLSLProgram; 

#ifdef _WIN32
	class GLWindow;
#else
	class SimpleGLXWindow;
#endif

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

    static vector<float> calculateNormalMatrix(const float* modelviewMatrix);
    
    void positionMonsters(int numMonsters);

#ifdef _WIN32
    void setWindow(GLWindow* window) {
        m_window = window;
    }
#else
    void setWindow(SimpleGLXWindow* window) {
        m_window = window;
    }
#endif

private:
    Terrain m_terrain;
    GLSLProgram* m_terrainProgram;
    GLSLProgram* m_modelProgram;

    MD2Model m_model;
    std::vector<Monster> m_ogros;
    
    TargaImage m_ogroTexture;
    GLuint m_ogroTextureID;

    float m_rotationAngle;

    Frustum m_frustum;

#ifdef _WIN32
    GLWindow* m_window;
#else
	SimpleGLXWindow* m_window;
#endif

    bool m_frustumCullingEnabled;

    float m_FPS;
    void updateFPS(float dt);
};

#endif
