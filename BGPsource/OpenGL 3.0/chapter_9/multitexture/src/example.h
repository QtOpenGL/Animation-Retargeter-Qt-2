#ifndef _EXAMPLE_H
#define _EXAMPLE_H

#include <vector>
#include <string>
#include <stdlib.h>
#include "terrain.h"
#include "targa.h"

#include "tree.h"

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

    static vector<float> calculateNormalMatrix(const float* modelviewMatrix);
    
    void positionTrees(float heightThreshold, float percentage);
private:
    Terrain m_terrain;
    GLSLProgram* m_GLSLProgram;
    GLSLProgram* m_waterProgram;

    
    TargaImage m_waterTexture;
    

    
    GLuint m_waterTexID;

    vector<Tree> m_trees;

    float m_rotationAngle;
};

#endif
