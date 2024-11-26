#include "camera.h"


Camera::Camera():x(0.0f),y(0.0f){

}

float Camera::getX(){
    return x;
}

float Camera::getY(){
    return y;
}

void Camera::setX(float x){
    this->x=x;
}

void Camera::setY(float y){
    this->y=y;
}

void Camera::moveUp(float step){
    this->y += step;
}

void Camera::moveDown(float step){
    this->y -= step;
}

void Camera::moveLeft(float step){
    this->x -= step;
}

void Camera::moveRight(float step){
    this->x += step;
}


void Camera::update() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float left = -180.0f  + this->x;
    float right = 180.0f  + this->x;
        float bottom = -90.0f  + this->y;
        float top = 90.0f  + this->y;
        glOrtho(left, right, bottom, top, -1.0f, 1.0f);
}
