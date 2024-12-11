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

void Camera::moveUp3D(float step){
    this->position += step*this->up;
}

void Camera::moveDown3D(float step){
    this->position -= step*this->up;
}

void Camera::moveFront3D(float step){
    this->position += step*this->forward;
}

void Camera::moveBack3D(float step){

    this->position -= step*this->forward;
}

void Camera::moveLeft3D(float step){
    this->position += step*this->right;
}

void Camera::moveRight3D(float step){
    this->position -= step*this->right;
}

void Camera::setZ(float zChange) {
    float scale = 1.0f + this->speedFactor;
    this->position[2] *= (zChange > 0) ? scale : 1.0f / scale;
    this->position[2] = std::max(this->position[2], 0.001f);
}


float Camera::getZ(){
    return position[2];
}

float Camera::getVertAng(){
    return VertAng;
}

float Camera::getHorAng(){
    return HorAng;
}

void Camera::rotateHorAng(float angle){
    this->HorAng+=angle;
    if(HorAng > 360){
        HorAng -=  360;
    }
    if(HorAng < 0){
        HorAng += 360;
    }
}

void Camera::rotateVertAng(float angle){
    this->VertAng+=angle;
    if(VertAng > 89.9){
        VertAng =  89.9;
    }
    if(VertAng < -89.9){
        VertAng = 89.9;
    }
}

QMatrix4x4 Camera::getViewMatrix() {
    // Convertir les angles de rotation en radians
    float pitchRad = glm::radians(this->VertAng); // Rotation autour de X
    float rollRad = glm::radians(this->HorAng);   // Rotation autour de Z

    glm::vec3 forward;
    forward.x = cos(pitchRad) * sin(rollRad);
    forward.z = sin(pitchRad);
    forward.y = cos(pitchRad) * cos(rollRad);

    // Calculer les autres axes (right, up) à partir de la direction forward
    glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), forward)); // Calculer l'axe droit (X)
    glm::vec3 up = glm::normalize(glm::cross(forward, right));

    // Récupérer la position de la caméra
    glm::vec3 position = this->getPosition();

    // Définir la cible à une certaine distance devant la caméra
    glm::vec3 target = position + forward;

    // Construction de la matrice de vue avec QMatrix4x4
    QVector3D cameraPos(position.x, position.y, position.z);
    QVector3D targetQ3D(target.x, target.y, target.z);
    QVector3D upQ3D(up.x, up.y, up.z);

    // Mettre à jour les directions locales de la caméra (facultatif)
    this->forward = forward;
    this->right = right;
    this->up = up;

    // Créer la matrice de vue en utilisant lookAt
    QMatrix4x4 viewMatrix;
    viewMatrix.lookAt(cameraPos, targetQ3D, upQ3D);

    return viewMatrix;
}

void Camera::update2D() {

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


