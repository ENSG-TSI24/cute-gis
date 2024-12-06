#include "camera.h"
#include <GL/glu.h>
#include <cmath>


Camera::Camera():position(0.0f,0.0f,1.0f){
    this->speedFactor = 1.2f; // defaut
}


void Camera::resetCamera(){
    this->setX(0.0);
    this->setY(0.0);
    this->setZ(1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
    this->position[1] += step*(position[2]*0.1);
}

void Camera::moveDown(float step){
    this->position[1] -= step*(position[2]*0.1);
}

void Camera::moveLeft(float step){
    this->position[0] -= step*(position[2]*0.1);
}

void Camera::moveRight(float step){
    this->position[0] += step*(position[2]*0.1);
}

void Camera::setZ(float zChange) {
    float scale = 1.0f + this->speedFactor;
    this->position[2] *= (zChange > 0) ? scale : 1.0f / scale;
    this->position[2] = std::max(this->position[2], 0.001f);

}


float Camera::getZ(){
    return position[2];
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

    // Passer en mode projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Calculer les limites de la projection orthographique
    float aspectRatio = static_cast<float>(renderer_width) / static_cast<float>(renderer_height);

    float verticalFOV = 45.0f*(M_PI / 180.0f); // Exemple en degrés, remplacez par votre FOV vertical
    float horizontalFOV = 2.0f * atan(tan(verticalFOV * 0.5f) * aspectRatio);


    float halfWidth = position[2] * tan(horizontalFOV/2); // Ajuster la largeur selon le zoom.
    float halfHeight = position[2] * tan(verticalFOV/2); // Ajuster la hauteur selon le zoom

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
    float FoV = 45.0f;
    float FoVRadian = FoV * M_PI / 180.0;
    float aspectRatio = 1.0f;

    // Calculer le centre de la bounding box
    float centerX = (bbox.minX + bbox.maxX) / 2.0f;
    float centerY = (bbox.minY + bbox.maxY) / 2.0f;

    // Calculer la taille de la bounding box
    float width = bbox.maxX - bbox.minX;
    float height = bbox.maxY - bbox.minY;

    // Positionner la caméra au centre
    this->position[0] = centerX;
    this->position[1] = centerY;

    // Adapter la distance (position[2]) pour inclure la bounding box
    // Choisir la plus grande dimension selon le ratio d'aspect
    float effectiveFoV = (aspectRatio > 1.0f) ? FoVRadian : FoVRadian / aspectRatio;
    float dimension = (aspectRatio > 1.0f) ? width : height;

    // Calculer la distance Z pour inclure la bounding box
    this->position[2] = dimension / (2.0f * tan(effectiveFoV / 2.0f));
}



glm::vec3 Camera::getPosition() {
    return position;
}

int Camera::getRHeight(){
    return this->renderer_height;
}

int Camera::getRWidth(){
    return this->renderer_width;
}

void Camera::setRHeight(int height){
    this->renderer_height=height;
}

void Camera::setRWidth(int width){
    this->renderer_width=width;
}

