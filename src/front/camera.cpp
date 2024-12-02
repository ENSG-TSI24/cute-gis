#include "camera.h"


Camera::Camera():x(0.0f),y(0.0f),zoom(1.0f){

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
    this->y += step/zoom;
}

void Camera::moveDown(float step){
    this->y -= step/zoom;
}

void Camera::moveLeft(float step){
    this->x -= step/zoom;
}

void Camera::moveRight(float step){
    this->x += step/zoom;
}

void Camera::setZoom(float zoomChange) {
    float speedFactor = 0.2f;
    float scale = 1.0f + speedFactor * this->zoom;

    this->zoom += zoomChange * scale;
    this->zoom = std::max(this->zoom, 0.1f);
}

float Camera::getZoom(){
    return zoom;
}

glm::vec3 Camera::getPosition() {
    return glm::vec3(x, y, 0.0f);
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
void Camera::update() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float left = -180.0f /zoom + this->x;
    float right = 180.0f /zoom + this->x;
    float bottom = -90.0f /zoom + this->y;
    float top = 90.0f /zoom + this->y;
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
    this->x = centerX;
    this->y = centerY;

    // Ajuster le zoom pour inclure la boîte
    float zoomX = 360.0f / width;
    float zoomY = 180.0f / height;
    this->zoom = std::min(zoomX, zoomY); // Garder le même facteur pour X et Y
}

