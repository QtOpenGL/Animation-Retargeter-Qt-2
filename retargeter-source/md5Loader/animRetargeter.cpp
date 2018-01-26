//
//  animRetargeter.cpp
//  DrawEngine
//
//  Created by Alex Handby on 28/07/2015.
//  Copyright (c) 2015 Alex Handby. All rights reserved.
//

#include "animRetargeter.h"

void animRetargeter::buildBindPose(Skeleton &bind){
    
    
    for(int i = 0; i < bind.size(); i++){
        //if you have a parent
        if(bind[i].getParent() > -1){
            //retrieve the local position of the bindPose.
            joint * parent = &bind[bind[i].getParent()];
            quaternion pInv = parent->getObjectOrient().getConjugate();
            
            bind[i].setLocalPos(bind[i].getObjectPos() - parent->getObjectPos());
            vec3 pos = pInv.rotateVec(bind[i].getLocalPos()); //localpos == object pos - parent position at this point.
            bind[i].setLocalPos(pos);
            bind[i].setLocalOrient(pInv * bind[i].getObjectOrient());
        }
        else
            bind[i].setLocalOrient(bind[i].getObjectOrient());
    }
    
}

quaternion animRetargeter::calcRotationBetweenVecs(vec3 & A, vec3 & B){
    
    quaternion ident;
    ident.becomeIdentity();
    
    //std::cout<< "\n";
    float angle = glm::dot(glm::normalize(A), glm::normalize(B));
    
    if(isnan(angle))
        return ident;
    
    angle = acos(angle);
    angle = angle * (180/M_PI);
    vec3 axis = glm::cross(A, B);
    
    if(glm::length(axis))
        axis = glm::normalize(axis);
    
    quaternion rotation(axis, angle);
    
    vec3 result = rotation.rotateVec(A);
    
    
    return rotation;
    
}

bool animRetargeter::inVector(int target, vector<int> vector){
    
    for(int i = 0; i < vector.size(); i++)
        if(vector[i] == target) return true;
    
    return false;
}

Anim* animRetargeter::retargetAnim(Anim * anim, AnimEntity * ent, map<int, int> * userMap, vector<int> roots, Skeleton bind){
    
    Anim * animation  = new Anim;
    
    Skeleton skel;
    skel.copy_data(ent->getBindPose());
    buildBindPose(skel);
    
    for(int j = 0; j < anim->getSkeletons().size(); j++){
        
        Skeleton skel2;
        skel2.copy_data(anim->getSkeletons()[j]);
        
        Skeleton nskel;
        nskel.copy_data(skel);
        
        nskel[0].setLocalOrient(skel2[0].getLocalOrient());
        
        nskel.recomputeObjectSpace();
        
        for(int i = 1; i < nskel.size(); i++){
            int p = nskel[i].getParent();
            

            if(userMap->count(i) && !nskel[p].beenComputed() && !inVector(i, roots)){
                
                
                int aj = userMap->find(i)->second;
                
                int gp = nskel[p].getParent();
                int p2 = skel2[aj].getParent();
                                
                vec3 rotatedLocalAnim = skel2[aj].getObjectPos() - skel2[p2].getObjectPos();
                vec3 boneDir = glm::normalize(rotatedLocalAnim);
                vec3 OLR = nskel[p].getObjectOrient().rotateVec(nskel[i].getLocalPos());
                
                float boneLength = glm::length(OLR);
                
                vec3 localRotated = boneDir * boneLength;
                
                //cout << nskel[i].getName() << " height: " << nskel[i].getHeight() << endl;
                
                quaternion diff = calcRotationBetweenVecs(OLR, localRotated);
                
                quaternion objOrient = diff * nskel[p].getObjectOrient();
                
                
                //------------------------------
                
                //retrieve the local position of the bindPose.
                quaternion pInv;
                quaternion local;
                quaternion test;
                
                if(gp > -1){
                    joint * gparent = &nskel[gp];
                    pInv = gparent->getObjectOrient().getConjugate();
                    
                    local = pInv * objOrient; //subtract child orient from the parent.
                    nskel[p].setLocalOrient(local);
                    test = gparent->getObjectOrient() * local;
                }else{
                    nskel[p].setLocalOrient(nskel[p].getObjectOrient());
                    test = nskel[p].getObjectOrient();
                }
                nskel[p].setComputed(true);
                //nskel.recomputeObjectSpace();
            }
            else if(userMap->count(i) && !nskel[p].beenComputed()){
                
                //cout << i << " " << nskel[i].getName() << endl;
                int aj = userMap->find(i)->second;
                int ap = skel2[aj].getParent();
                int agp = skel2[ap].getParent();
                quaternion diff = skel2[agp].getObjectOrient().getConjugate() * skel2[ap].getObjectOrient();
                nskel[p].setLocalOrient(nskel[p].getLocalOrient() * diff);
                nskel[p].setComputed(true);
                //nskel.recomputeObjectSpace();
            }
            
            nskel.recomputeObjectSpace();
            
        }
    
        /*if(j > 0){
            vec3 origDiff = skel2[0].getObjectPos() - anim->getSkeletons()[0][0].getObjectPos();
            vec3 bodyDiff =  skel2[1].getObjectPos() - anim->getSkeletons()[0][1].getObjectPos();
            vec3 hipsDiff = skel2[2].getObjectPos() - anim->getSkeletons()[0][2].getObjectPos();
            
            std::cout << bodyDiff.x << " " << bodyDiff.y << " " << bodyDiff.z << std::endl;
            
            //nskel[0].setLocalPos(nskel[0].getLocalPos() += origDiff);
            nskel[1].setLocalPos(nskel[1].getLocalPos() += bodyDiff);
            //nskel[3].setLocalPos(nskel[3].getLocalPos() += hipsDiff);
        }
        
        nskel.recomputeObjectSpace();*/
        
        animation->getSkeletons().push_back(nskel);
        
    }
    return animation;
    
}