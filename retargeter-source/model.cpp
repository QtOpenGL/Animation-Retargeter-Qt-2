//
//  model.cpp
//  DrawEngine
//
//  Created by Alex Handby on 15/07/2015.
//  Copyright (c) 2015 Alex Handby. All rights reserved.
//

#include "model.h"

using std::cout;
using std::endl;

/*
 * Model
 */
Model::Model( vector<GLuint> nIndices, vector<float> nTexture ) : inBuffer(false), texture(nTexture), indices(nIndices) {
    
}

Model::Model(const Model & m) : inBuffer(m.inBuffer), indices(m.indices), texture(m.texture){}

 Model::~Model(){}

void Model::setBufferIDs (GLuint newIB, GLuint newTB){
    inBuffer = true;
    IB = newIB;
    TB = newTB; 
}

/*
 * StaticModel
 */
StaticModel::StaticModel( vector<GLuint> nIndices, vector<float> nTexture, vector<float> nVerts )
: verts(nVerts), Model( nIndices, nTexture ){}

StaticModel::StaticModel(const StaticModel & m): verts(m.verts), Model(m){}

void StaticModel::setBufferIDs (GLuint newVB, GLuint newIB, GLuint newTB) {
    VB = newVB;
    Model::setBufferIDs(newIB, newTB);
}

/*
 * AnimModel
 */
AnimModel::AnimModel( string nName, vector<GLuint> nIndices, vector<float> nTexture, vector<vert> nVertices, vector<weight> nWeights ) : name(nName), weights(nWeights), vertices(nVertices), Model(nIndices, nTexture){}

AnimModel::AnimModel(const AnimModel & m): name(m.name), Model(m){}

AnimModel::~AnimModel(){}

vector<float> AnimModel::computeVerts(Skeleton & skel){
    
    vector<float> glverts;
    
    for(int vertIndex = 0; vertIndex < vertices.size(); vertIndex++){
        
        vec3 finalPos(0, 0, 0);
        
        //iterate through each weight associated with this vert.
        for(int weightIndex = 0; weightIndex < vertices[vertIndex].getWeightCount(); ++weightIndex){
            
            weight currentW = weights[vertices[vertIndex].getStartWeight() + weightIndex];
            
            joint & temp = skel[currentW.getJoint()];
            
            vec3 rotatedPos = temp.getObjectOrient().rotateVec(currentW.getPos());
            
            //sum all the weights multiplied by their bias
            finalPos += (temp.getObjectPos() + rotatedPos) * currentW.getBias();
            
            
        }
        
        glverts.push_back(finalPos.x);
        glverts.push_back(finalPos.y);
        glverts.push_back(finalPos.z);
    }
    
    return glverts;
    
}

/*
 * Anim
 */
Anim::Anim(const Anim & a): name(a.name), frameRate(a.frameRate), Skeletons(a.Skeletons) {}


/*
 * Skeleton
 */
void Skeleton::recomputeObjectSpace(){
    
    for(int i = 0; i < joints.size(); i++){
        
        int parentIndex = joints[i].getParent();
    
        //does the joint have a parent?
        if(parentIndex > -1){
        
            //every joint is computed in order.
            joint* parent = &joints[parentIndex];
        
            //compute the object space pos of the joint.
            vec3 pos = parent->getObjectOrient().rotateVec(joints[i].getLocalPos());
            joints[i].setObjectPos(pos + parent->getObjectPos());
        
            //compute the object space orientation of the joint.
            joints[i].setObjectOrient(parent->getObjectOrient() * joints[i].getLocalOrient());
            //joints[i].getObjectOrient().normalize();
        
        }else{
            joints[i].setObjectPos(joints[i].getLocalPos());
            joints[i].setObjectOrient(joints[i].getLocalOrient());
        }
        

    }

}

void Skeleton::printJoints(){
    for(int i = 0; i < joints.size(); i++){
        std::cout << i << ": " << joints[i].getObjectOrient().toString() << joints[i].getObjectPos().x << " " << joints[i].getObjectPos().y << " " << joints[i].getObjectPos().z << std::endl;
    }
}

void Skeleton::computeHeight(){
    
    for(int i = 0; i < joints.size(); i++){
        compAux(joints[i].getParent(), 1);
    }
    
    for(int i = 0; i < joints.size(); i++){
        
        if(joints[i].getHeight() >= 4)
            heights.push_back(i);
    }
}

void Skeleton::compAux(int parent, int height){

    if(parent > -1 && joints[parent].getHeight() < height){
        joints[parent].setHeight(height);
        //cout << joints[parent].getName() << " height: " << joints[parent].getHeight() << endl;
        compAux(joints[parent].getParent(), height + 1);
    }
    
}


/*
 * ModelGenerator
 */
StaticModel * modelGenerator::generateTriangle(){
    
    vector<float> verts;
    vector<GLuint> indices;
    vector<float> texture;
    
    verts = {
                            -1.0f, -1.0f, -1.0f, //left corner
                            1.0f, -1.0f, -1.0f, //right corner
                            0.0f, 1.0f, -1.0f //top
                            };
    
     indices = {0, 2, 1};
    
     texture = {
                            0.0f, 0.0f, //left corner
                            1.0f, 0.0f, //right corner
                            0.5f, 1.0f //top
                            };
    
    StaticModel * temp (new StaticModel(indices, texture, verts));
    
    return temp;
}

StaticModel * modelGenerator::generateBone(){
    
    vector<float> verts;
    vector<GLuint> indices;
    vector<float> texture;
    
    verts = {
        0, 3, 0, //top front
        1, 1, 1,
        -1, 1, 1,
        
        0, 3, 0, //top left
        -1, 1, 1,
        -1, 1, -1,
        
        0, 3, 0, //top right
        1, 1, -1,
        1, 1, 1,
        
        0, 3, 0, //top back
        -1, 1, -1,
        1, 1, -1,
        
        -1, 1, 1, //bottom front
        1, 1, 1,
        0, 0, 0,
        
        -1, 1, -1, //left bottom
        -1, 1, 1,
        0, 0, 0,
        
        1, 1, 1, //right bottom
        1, 1, -1,
        0, 0, 0,
        
        1, 1, -1, //back bottom
        -1, 1, -1,
        0, 0, 0
    };
    
    indices = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        15, 16, 17,
        18, 19, 20,
        21, 22, 23
    };
    
    texture = {
        0.5f, 1.0f, //top
        1.0f, 0.0f, //right corner
        0.0f, 0.0f, //left corner
        
        0.5f, 1.0f, //top
        1.0f, 0.0f, //right corner
        0.0f, 0.0f, //left corner
        
        0.5f, 1.0f, //top
        1.0f, 0.0f, //right corner
        0.0f, 0.0f, //left corner
        
        0.5f, 1.0f, //top
        1.0f, 0.0f, //right corner
        0.0f, 0.0f, //left corner
        
        0.5f, 1.0f, //top
        1.0f, 0.0f, //right corner
        0.0f, 0.0f, //left corner
        
        0.5f, 1.0f, //top
        1.0f, 0.0f, //right corner
        0.0f, 0.0f, //left corner
        
        0.5f, 1.0f, //top
        1.0f, 0.0f, //right corner
        0.0f, 0.0f, //left corner
        
        0.5f, 1.0f, //top
        1.0f, 0.0f, //right corner
        0.0f, 0.0f //left corner
    };
    
    StaticModel * temp (new StaticModel(indices, texture, verts));
    
    return temp;
}

StaticModel * modelGenerator::generateCube(){
    
    vector<float> verts;
    vector<GLuint> indices;
    vector<float> texture;
    
    verts = {
                        -1, 1, 1, //front
                        1, 1, 1,
                        1, -1, 1,
                        -1, -1, 1,
        
                        -1, 1, -1, //left
                        -1, 1, 1,
                        -1, -1, 1,
                        -1, -1, -1,
        
                         1, 1, 1, //right
                         1, 1, -1,
                         1, -1, -1,
                         1, -1, 1,
        
                        1, 1, -1, //back
                        -1, 1, -1,
                        -1, -1, -1,
                        1, -1, -1,
        
                        -1, 1, -1, //top
                        1, 1, -1,
                        1, 1, 1,
                        -1, 1, 1,
        
                        -1, -1, 1, //bottom
                        1, -1, 1,
                        1, -1, -1,
                        -1, -1, -1
        
                        };
    
    indices = {
                                0, 1, 2, //front
                                2, 3, 0,
        
                                4, 5, 6, //left
                                6, 7, 4,
        
                                8, 9, 10, //right
                                10, 11, 8,
        
                                12, 13, 14, //back
                                14, 15, 12,
        
                                16, 17, 18, //top
                                18, 19, 16,
        
                                20, 21, 22, //bottom
                                22, 23, 20
        
                            };
    
    texture = {
                            0, 1,
                            1, 1,
                            1, 0,
                            0, 0,
        
                            0, 1,
                            1, 1,
                            1, 0,
                            0, 0,
        
                            0, 1,
                            1, 1,
                            1, 0,
                            0, 0,
        
                            0, 1,
                            1, 1,
                            1, 0,
                            0, 0,

                            0, 1,
                            1, 1,
                            1, 0,
                            0, 0,
        
                            0, 1,
                            1, 1,
                            1, 0,
                            0, 0
                            };
    
    StaticModel * temp (new StaticModel(indices, texture, verts));
    return temp;

}