//
//  camera.cpp
//  DrawEngine
//
// This class is based on a video playlist by Jamie King.
// https://www.youtube.com/playlist?list=PLRwVmtr-pp06qT6ckboaOhnm9FxmzHpbY
//

#include <iostream>
#include "camera.h"


/*===============================
 * Camera
 * @description: the view point of the user.
 *===============================*/

/*
 * MoveForward
 * Moves the camera forward
 */
void camera::moveForward(){
    position += 5.0f * viewDirection;
}

/*
 * MoveBackward 
 * Moves the camera backward
 */
void camera::moveBackward(){
    position += -5.0f * viewDirection;
}

/*
 * MoveUp
 * Moves the camera upward
 */
void camera::moveUp(){
    position += 5.0f * up;
}

/*
 * MoveDown
 * Moves the camera downward
 */
void camera::moveDown(){
    position += -5.0f * up;
}

/*
 * LookLeft
 * Rotates the camera to the left
 */
void camera::lookLeft(){
    viewDirection = glm::mat3(glm::rotate(0.1f, up)) * viewDirection;
}

/*
 * LookRight
 * Rotates the camera to the left
 */
void camera::lookRight(){
    viewDirection = glm::mat3(glm::rotate(-0.1f, up)) * viewDirection;
}

/*
 * LookRight
 * Rotates the camera to the left
 */
void camera::strafeLeft(){
    glm::vec3 strafeDirection = glm::cross(viewDirection, up);
    position += -5.0f * strafeDirection;
}

/*
 * StrafeRight
 * Moves the camera to the right
 */
void camera::strafeRight(){
    glm::vec3 strafeDirection = glm::cross(viewDirection, up);
    position += 5.0f * strafeDirection;
}

/*
 * LookUp
 * Rotates the camera upward
 */
void camera::lookUp(){
    glm::vec3 toRotateAround = glm::cross(viewDirection, up);
    viewDirection = glm::mat3(glm::rotate(0.1f, toRotateAround)) * viewDirection;
}

/*
 * LookUp
 * Rotates the camera downward
 */
void camera::lookDown(){
    glm::vec3 toRotateAround = glm::cross(viewDirection, up);
    viewDirection = glm::mat3(glm::rotate(-0.1f, toRotateAround)) * viewDirection;
}

