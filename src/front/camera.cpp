#include "camera.h"


Camera::Camera():position(0.0f,0.0f,1.0f){

}

float Camera::getX(){
    return position[0];
}

float Camera::getY(){
    return position[1];
}

void Camera::setX(float x){
    this->position[0]=x;
}

void Camera::setY(float y){
    this->position[1]=y;
}

void Camera::moveUp(float step){
    this->position[1] += step/position[2];
}

void Camera::moveDown(float step){
    this->position[1] -= step/position[2];
}

void Camera::moveLeft(float step){
    this->position[0] -= step/position[2];
}

void Camera::moveRight(float step){
    this->position[0] += step/position[2];
}

void Camera::setZ(float zChange) {
    float speedFactor = 0.2f;

    this->position[2] += zChange * speedFactor;
    this->position[2] = std::max(this->position[2], 0.000000000000000000001f);
    std::cout<<position[2]<<std::endl;
}

float Camera::getZ(){
    return position[2];
}

void Camera::update() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float left = -180.0f /position[2] + this->position[0];
    float right = 180.0f /position[2] + this->position[0];
    float bottom = -90.0f /position[2] + this->position[1];
    float top = 90.0f /position[2] + this->position[1];
    glOrtho(left, right, bottom, top, -1.0f, 1.0f);
}

void Camera::centerOnBoundingBox(const BoundingBox& bbox) {
    // Calculer le centre
    float centerX = (bbox.minX + bbox.maxX) / 2.0f;
    float centerY = (bbox.minY + bbox.maxY) / 2.0f;

    // Calculer la taille de la boîte englobante
    float width = bbox.maxX - bbox.minX;
    float height = bbox.maxY - bbox.minY;

    // Ajuster la caméra
    this->position[0] = centerX;
    this->position[1] = centerY;

    // Ajuster le zoom pour inclure la boîte
    float zoomX = 360.0f / width;
    float zoomY = 180.0f / height;
    this->position[2] = std::min(zoomX, zoomY); // Garder le même facteur pour X et Y
}

glm::vec3 Camera::getPosition() {
    return position;
}

QMatrix4x4 Camera::getViewMatrix() {
    glm::vec3 position = this->getPosition();
    QVector3D cameraPos(position.x, position.y, 3.0f);
    QVector3D target(position.x, position.y, 0.0f);
    QVector3D upVector(0.0f, 1.0f, 0.0f);

    QMatrix4x4 viewMatrix;
    viewMatrix.lookAt(cameraPos, target, upVector);
    return viewMatrix;
}

QMatrix4x4 Camera::getModelViewMatrix(const QMatrix4x4& modelMatrix) {
    QMatrix4x4 viewMatrix = getViewMatrix();
    return viewMatrix * modelMatrix;
}
