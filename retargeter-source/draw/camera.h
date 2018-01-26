//
//  camera.h
//  experiment
//
//  Created by Alex Handby on 14/06/2015.
//  Copyright (c) 2015 Alex Handby. All rights reserved.
//
#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <stdio.h>
#include "glm.hpp"
#include "transform.hpp"

/*===============================
 * Camera
 * @description: the view point of the user.
 *===============================*/

class camera {

private:
    glm::vec3 position;
    glm::vec3 viewDirection;
    glm::vec3 up;
    int mousex;
    int mousey;
    glm::vec2 oldMousePosition;
    
    
    
public:
    camera() : viewDirection(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f){}
    camera(glm::vec3 newPos) : viewDirection(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f){ this->position = newPos; }
    
     glm::mat4 getWorldToViewMatrix() const{
        return glm::lookAt(position, position + viewDirection, up);
    }
    
    void moveForward();
    void moveBackward();
    void moveUp();
    void moveDown();
    void lookLeft();
    void lookRight();
    void strafeLeft();
    void strafeRight();
    void lookUp();
    void lookDown();
    
};

 
 

#endif // CAMERA_H_INCLUDED