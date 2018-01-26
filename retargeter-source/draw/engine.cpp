/*
 * Low Level 
 *
 */
#include <iostream>
#include <vector>
#include <string>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "glslshader.h"
#include "glm.hpp"
#include "matrix_transform.hpp"
#include "transform.hpp"
#include "targa.h"
#include "engine.h"
#include "entity.h"
#include "camera.h"

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

/*===============================
 * DrawEngine
 * @description: This class handles all the Opengl code/rendering
 * responsibilities for the program.
 *===============================*/

/*
 * DrawEngine Constructor
 * @param windowWidth - the opengl context width.
 * @param windowHeight - the opengl context height.
 */
DrawEngine::DrawEngine(int windowWidth, int windowHeight, camera *newCam){
    
    nxtTexNum = GL_TEXTURE0;
    
    //create the perspecitve matrix.
    resize(windowWidth, windowHeight);
    
    loadShaders();
    
    cam = newCam;
    
    //OpenGL setup
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glEnable(GL_TEXTURE_2D);
    
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    
}

/*
 * ~DrawEngine
 */
DrawEngine::~DrawEngine(){
    deleteResources();
    deleteTextures();
}

/*
 * resize
 * @param windowWidth - the opengl context width.
 * @param windowHeight - the opengl context height.
 */
void DrawEngine::resize(int windowWidth, int windowHeight){
    glViewport(0, 0, windowWidth, windowHeight);
    projMatrix = glm::perspective(45.0f, (float) windowWidth/windowHeight, 0.1f, 1000.0f);
}

/*
 * storeTexture
 * @param windowWidth - the opengl context width.
 * @return GLuint textureID - the ID of the texture being stored 
 *  in the graphics card.
 */
GLuint DrawEngine::storeTexture(string filepath){
	    
	TargaImage modelTexture;
	    
	if (!modelTexture.load(filepath))
    {
        std::cerr << "Could not load the model texture" << std::endl; //could be replace with an exception.
        return false;
    } 
    
	GLuint textureBuffer;
	glGenTextures(1, &textureBuffer);
    glActiveTexture(GL_TEXTURE0); //GL_TEXTURE0 or TEXTURE1 e.c.t defined by how the class handles this
    glBindTexture(GL_TEXTURE_2D, textureBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, modelTexture.getWidth(),
                 modelTexture.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 modelTexture.getImageData());
    
   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, modelTexture.getWidth(),
                      modelTexture.getHeight(), GL_RGB, GL_UNSIGNED_BYTE,
                      modelTexture.getImageData());
    

    
    TexIDs.push_back(textureBuffer);
                 
    return textureBuffer;
}

/*
 * loadShaders
 * load and compile the shaders into executable code for the graphics card.
 * @return bool success - whether the compilation was successful.
 */
bool DrawEngine::loadShaders(){
    
    textureShader = new GLSLProgram("shaders/basic-fixed.vert", "shaders/basic-fixed.frag");
    
    if(!textureShader->initialize()){
        cerr << "Could not initialise shader program" << endl;
        return false;
    }
    
    //bind the attributes
    textureShader->bindAttrib(0, "a_Vertex");
    textureShader->bindAttrib(1, "s_vTexCoord");
    
    textureShader->linkProgram(); //relink program
    textureShader->bindShader(); //enable the shader
    
    return true;
}


/*
 * BeginDraw
 * prepare the gl context for rendering
 */
void DrawEngine::BeginDraw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*
 * BufferedDrawTex
 * Send an animated entity to the graphics card to be rendered to the screen
 * @param AnimEntity ent - the entity to be rendered.
 */
void DrawEngine::BufferedDrawTex(AnimEntity &ent){
    
    for(int i = 0; i < ent.getSubEntities().size(); i++ ){
        AnimSubEntity & ant = ent.getSubEntities()[i];
        
        if(!ant.isVisible())
            continue;
        
        //is there data loaded into the graphics card already?
        if (!ant.getModel()->isInBuffer())
            sendDataToBufferTex(ant);
        
        else if(ent.frameChanged())
            resendVertsToBuffer(ant);
    
    
        finalMatrix = projMatrix * cam->getWorldToViewMatrix() * ent.getPos() * ent.getOrient();
    
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ant.getTexBID());
    
        textureShader->sendUniform4x4("projection_matrix", &finalMatrix[0][0], GL_FALSE);
        textureShader->sendUniform("texture", 0.0f);
    
        //enable attrib arrays.
        glEnableVertexAttribArray(0); //Enable the vertex attribute
        glEnableVertexAttribArray(1); //Enable the texture coordinate attribute
    
        //texture code
        glBindBuffer(GL_ARRAY_BUFFER, ant.getModel()->getTB());
        glVertexAttribPointer((GLint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    
        //vertex code.
        glBindBuffer(GL_ARRAY_BUFFER, ant.getVB());
        glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
        //index code.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ant.getModel()->getIB());
    
    
        glDrawElements (GL_TRIANGLES, ant.getModel()->getIndices().size(), GL_UNSIGNED_INT, 0);
    
        //disable attrib arrays.
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        
        
    }
    
    if(ent.frameChanged())
        ent.changedOff();

}

/*
 * BufferedDrawTex
 * Send a skeleton to be drawn by the graphics card.
 * @param skeleton - the entity to be rendered. 
 * @param entity - static entity to represent the position of a bone.
 * @param masterPos - the position of the skelelton's entity.
 */
void DrawEngine::BufferedDrawSkel(Skeleton & skel, StaticEntity & ent, mat4 masterPos, mat4 masterRot){
    
    for(int i = 0; i < skel.size(); i++){
        
        if(!ent.getModel()->isInBuffer())
            sendDataToBufferTex(ent);
        
        
        mat4 pos = glm::translate(skel[i].getObjectPos());
        mat4 rot = skel[i].getObjectOrient().toMatrix();
        
        finalMatrix = projMatrix * cam->getWorldToViewMatrix() * masterPos * masterRot * pos * rot;
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ent.getTexBID());
        
        textureShader->sendUniform4x4("projection_matrix", &finalMatrix[0][0], GL_FALSE);
        textureShader->sendUniform("texture", 0.0f);
        
        //enable attrib arrays.
        glEnableVertexAttribArray(0); //Enable the vertex attribute
        glEnableVertexAttribArray(1); //Enable the texture coordinate attribute
        
        //texture code
        glBindBuffer(GL_ARRAY_BUFFER, ent.getModel()->getTB());
        glVertexAttribPointer((GLint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        
        //vertex code.
        glBindBuffer(GL_ARRAY_BUFFER, ent.getModel()->getVB());
        glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        //index code.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ent.getModel()->getIB());
        
        glDrawElements (GL_TRIANGLES, ent.getModel()->getIndices().size(), GL_UNSIGNED_INT, 0);
        
        //disable attrib arrays.
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        
    }

    
}

/*
 * BufferedDrawTex
 * Send a static entity to the graphics card to be rendered to the screen
 * @param StaticEntity ent - the entity to be rendered.
 */
void DrawEngine::BufferedDrawTex(StaticEntity &ent){
	
    
    if(!ent.getModel()->isInBuffer())
        sendDataToBufferTex(ent);
    
    
    finalMatrix = projMatrix * cam->getWorldToViewMatrix() * ent.getPos() * ent.getOrient();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ent.getTexBID());
    
    textureShader->sendUniform4x4("projection_matrix", &finalMatrix[0][0], GL_FALSE);
    textureShader->sendUniform("texture", 0.0f);
    
    //enable attrib arrays.
    glEnableVertexAttribArray(0); //Enable the vertex attribute
    glEnableVertexAttribArray(1); //Enable the texture coordinate attribute
    
    //texture code
    glBindBuffer(GL_ARRAY_BUFFER, ent.getModel()->getTB());
    glVertexAttribPointer((GLint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
   //vertex code.
    glBindBuffer(GL_ARRAY_BUFFER, ent.getModel()->getVB());
    glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    //index code.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ent.getModel()->getIB());
        
    glDrawElements (GL_TRIANGLES, ent.getModel()->getIndices().size(), GL_UNSIGNED_INT, 0);
    
    //disable attrib arrays.
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
     
}

/*
 * sendDataToBufferTex
 * Send a animated sub-entity to the graphics card.
 * @param AnimSubEntity ent - the entity to be rendered.
 */
void DrawEngine::sendDataToBufferTex(AnimSubEntity &ent){
    GLuint VB, IB, TB;
    
    glGenBuffers(1, &VB);
    glGenBuffers(1, &IB);
    glGenBuffers(1, &TB);
    
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ent.getVertices().size(), ent.getVertices().data(), GL_STATIC_DRAW);
    
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * ent.getModel()->getIndices().size(), ent.getModel()->getIndices().data(), GL_STATIC_DRAW);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, TB); //Bind the texture buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ent.getModel()->getTexture().size(), ent.getModel()->getTexture().data(), GL_STATIC_DRAW); //Send the data
    
    ent.getModel()->setBufferIDs(IB, TB); //want this is be a joined operation, genbuffer then set model flags
    ent.setBufferIDs(VB);
}

/*
 * resendVertsToBuffer
 * Send a animated subentity to the graphics card.
 * @param AnimSubEntity ent - the entity to be rendered.
 */
void DrawEngine::resendVertsToBuffer(AnimSubEntity &ent){
    
    glBindBuffer(GL_ARRAY_BUFFER, ent.getVB());
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ent.getVertices().size(), ent.getVertices().data(), GL_STATIC_DRAW);
    
}

/*
 * sendDataToBufferTex
 * Send a StaticEntity to the graphics card.
 * @param StaticEntity ent - the entity to be rendered.
 */
void DrawEngine::sendDataToBufferTex(StaticEntity &ent){
    
    GLuint VB, IB, TB;
    
    //send data to OpenGL
    glGenBuffers(1, &VB);
    glGenBuffers(1, &IB);
    glGenBuffers(1, &TB);
    
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ent.getModel()->getVerts().size(), ent.getModel()->getVerts().data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * ent.getModel()->getIndices().size(), ent.getModel()->getIndices().data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, TB); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ent.getModel()->getTexture().size(), ent.getModel()->getTexture().data(), GL_STATIC_DRAW); //Send the data
    
    ent.getModel()->setBufferIDs(VB, IB, TB); //want this to be a joined operation, genbuffer then set model flags
    
}

/*
 * deleteResources
 * ensure all dynamically allocated memory is freed.
 */
void DrawEngine::deleteResources(){
    delete textureShader;
}

/*
 * deleteTextures
 * free the texture memory stored on the graphics card
 */
void DrawEngine::deleteTextures(){
    glDeleteTextures((GLsizei)TexIDs.size(), TexIDs.data());
}
