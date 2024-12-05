#include "camera.h"
#include <GL/glu.h>
#include <cmath>


Camera::Camera():position(0.0f,0.0f,1.0f){
    this->speedFactor = 1.2f; // defaut
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
    float scale = 1.0f + this->speedFactor;
    this->position[2] *= (zChange > 0) ? scale : 1.0f / scale;
    this->position[2] = std::max(this->position[2], 0.1f);

    std::cout << "Camera Z position: " << this->position[2] << std::endl;
}


float Camera::getZ(){
    return position[2];
}

void Camera::update() {

    // Passer en mode projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Calculer les limites de la projection orthographique
    float FoV = 45.0f;
    float FoVRadian = FoV * M_PI / 180.0;

    float halfWidth = position[2] * tan(FoVRadian/2); // Ajuster la largeur selon le zoom.
    float halfHeight = position[2] * tan(FoVRadian/2); // Ajuster la hauteur selon le zoom

    float left = position[0] - halfWidth;   // Décaler à gauche
    float right = position[0] + halfWidth; // Décaler à droite
    float bottom = position[1] - halfHeight; // Décaler en bas
    float top = position[1] + halfHeight;   // Décaler en haut

    // Définir la matrice de projection orthographique
    glOrtho(left, right, bottom, top, -1.0f, 1.0f);

    // Passer en mode modèle
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /*
    // Configurer la projection 3D standard
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Vous pouvez ajuster les valeurs pour adapter la perspective
    gluPerspective(45.0f, 1.0f, 0.1f, 100.0f);  // Perspective classique pour 3D (par exemple)

    // Configurer la matrice de modèle/vue
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(0.0f, 0.0f, -position[2]);  // Caméra positionnée en Z

    QMatrix4x4 modelViewMatrix;
    modelViewMatrix.translate(-position[0], -position[1], 0.0f);  // Centrage de la caméra

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(modelViewMatrix.constData());
*/

}

void Camera::centerOnBoundingBox(const BoundingBox& bbox) {
    // Calculer le centre
    float centerX = (bbox.minX + bbox.maxX) / 2.0f;
    float centerY = (bbox.minY + bbox.maxY) / 2.0f;

    // Calculer la taille
    float width = bbox.maxX - bbox.minX;
    float height = bbox.maxY - bbox.minY;

    // Positionner la caméra
    this->position[0] = centerX;
    this->position[1] = centerY;

    // Adapter le zoom pour inclure la bounding box
    float zoomX = 360.0f / width;   // 360 correspond à 2x180, largeur de l'écran
    float zoomY = 180.0f / height; // 2x90, hauteur de l'écran
    this->position[2] = std::min(zoomX, zoomY); // Garder un zoom qui inclut la box tout en respectant les proportions
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
