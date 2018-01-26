//
//  engine.h
//  DrawEngine
//
//  Created by Alex Handby on 14/07/2015.
//  Copyright (c) 2015 Alex Handby. All rights reserved.
//


#ifndef DrawEngine_engine_h
#define DrawEngine_engine_h

#include <iostream>
#include <OpenGl/gl.h>
#include <OpenGL/glu.h>
#include <vector>
#include <string.h>

#include "targa.h"
#include "entity.h"
#include "glm.hpp"
#include "matrix_transform.hpp"
#include "transform.hpp"
#include "glslshader.h"
#include "camera.h"

using std::string;
using std::vector;
using glm::vec3;
using glm::mat4;

/*===============================
 * DrawEngine
 * @description: This class handles all the Opengl code/rendering
 * responsibilities for the program.
 *===============================*/

class DrawEngine {

private:
    vector<GLuint> TexIDs;
    GLenum nxtTexNum;
    GLSLProgram * textureShader;
    camera * cam;
    mat4 cameraMatrix;
    mat4 projMatrix;
    mat4 finalMatrix;

 
/*----------------- low level --------------------------*/
private:
    void deleteTextures(); //delete all the textures that have been sent to the GPU
    void deleteResources(); //delete all the resources allocated
    void sendDataToBufferTex(StaticEntity &ent);
    void sendDataToBufferTex(AnimSubEntity &ent);
    
    
/*---------------- High level ------------------------*/

public: 
    DrawEngine(int windowWidth, int windowHeight, camera * newCam);
    ~DrawEngine();
    void resize(int windowWidth, int windowHeight); //resizes the glviewport e.c.t
    void BufferedDrawTex(StaticEntity  &ent);
    void BufferedDrawTex(AnimEntity &ent);
    void BufferedDrawSkel(Skeleton &skel, StaticEntity &ent, mat4 masterPos, mat4 masterRot);
    void BeginDraw();
    GLuint storeTexture(string filepath); //takes a texture and loads it into the GPU.
    bool loadShaders();
    void resendVertsToBuffer(AnimSubEntity &ent);

};


#endif
