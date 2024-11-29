#include "camera.h"
#include <GL/gl.h>
#include <vector>


Camera::Camera()
    : x(0.0f), y(0.0f), zoomLevel(1.0f) {}

float Camera::getX() const {
    return x;
}

float Camera::getY() const {
    return y;
}

void Camera::setX(float newX) {
    x = newX;
}

void Camera::setY(float newY) {
    y = newY;
}

float Camera::getZoomLevel() const {
    return zoomLevel;
}

void Camera::setZoomLevel(float newZoom) {
    if (newZoom > 0.1f) { // Empêche un zoom trop faible ou négatif
        zoomLevel = newZoom;
    }
}

void Camera::moveUp(float step) {
    y += step / zoomLevel;
}

void Camera::moveDown(float step) {
    y -= step / zoomLevel;
}

void Camera::moveLeft(float step) {
    x -= step / zoomLevel;
}

void Camera::moveRight(float step) {
    x += step / zoomLevel;
}

void Camera::apply() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float left = -180.0f / zoomLevel + x;
    float right = 180.0f / zoomLevel + x;
    float bottom = -90.0f / zoomLevel + y;
    float top = 90.0f / zoomLevel + y;

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
    this->zoomLevel = std::min(zoomX, zoomY); // Garder le même facteur pour X et Y
}
