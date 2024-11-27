#include "camera.h"
#include <GL/gl.h>

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
