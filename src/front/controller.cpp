#include "controller.h"

Controller::Controller(QWidget* parent)
    : QWidget(parent), camera(), isDragging(false),is3DMode(false) {}

Camera& Controller::getCamera() {
    return camera; // Retourne la référence à la caméra
}

void Controller::ControllerwheelEvent(QWheelEvent* event) {
    float zoomStep = 1.0f;
    if (is3DMode){
        if (event->angleDelta().y() > 0) {

        } else if (event->angleDelta().y() < 0) {

        }
    }
    else{
        if (event->angleDelta().y() > 0) {
            camera.setZ(-zoomStep);
        } else if (event->angleDelta().y() < 0) {
            camera.setZ(zoomStep);
        }
        camera.update2D();
    }
    update();
}

void Controller::ControllerkeyPressEvent(QKeyEvent *event){
    float step = is3DMode ? 1.0f : 1.0f;
    if(!is3DMode){
        switch (event->key()) {
        case( Qt::Key_Up):
            this->camera.moveUp(step);
            break;
        case(Qt::Key_Down):
            this->camera.moveDown(step);
            break;
        case(Qt::Key_Left):
            this->camera.moveLeft(step);
            break;
        case(Qt::Key_Right):
            this->camera.moveRight(step);
            break;
        case(Qt::Key_Z):
            this->camera.moveUp(step);
            break;
        case(Qt::Key_Q):
            this->camera.moveLeft(step);
            break;
        case(Qt::Key_D):
            this->camera.moveRight(step);
            break;
        case(Qt::Key_S):
            this->camera.moveDown(step);
            break;
        }
        camera.update2D();
    }
    else{
        switch (event->key()) {
        case( Qt::Key_Control):
            isCtrlPressed = true;
            break;
        case( Qt::Key_Up):
            this->camera.moveUp(step);
            break;
        case(Qt::Key_Down):
            this->camera.moveDown(step);
            break;
        case(Qt::Key_Left):
            this->camera.moveLeft(step);
            break;
        case(Qt::Key_Right):
            this->camera.moveRight(step);
            break;
        case(Qt::Key_Z):
            this->camera.moveFront3D(step);
            break;
        case(Qt::Key_Q):
            this->camera.moveLeft3D(step);
            break;
        case(Qt::Key_D):
            this->camera.moveRight3D(step);
            break;
        case(Qt::Key_S):
            this->camera.moveBack3D(step);
            break;
        case(Qt::Key_Space):
            this->camera.moveUp3D(step);
            break;
        case(Qt::Key_W):
            this->camera.moveDown3D(step);
            break;

        }
    }
    update();
}

void Controller::ControllerkeyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        isCtrlPressed = false;
    }
}


void Controller::ControllerMousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = true;
        lastMousePosition = event->pos();
    }
}

void Controller::ControllerMouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
    }
}

void Controller::ControllerMouseMoveEvent(QMouseEvent* event) {
    if (isDragging) {
        float sensitivity = is3DMode ? 0.15f : 0.02f;
        QPoint currentMousePosition = event->pos();
        QPoint delta = currentMousePosition - lastMousePosition;
        if (is3DMode){
            if(isCtrlPressed){
                camera.rotateVertAng(delta.y() * sensitivity);
                camera.rotateHorAng(-delta.x() * sensitivity);
            }
            else{
                camera.moveRight3D(-delta.x() * sensitivity);
                camera.moveFront3D(delta.y() * sensitivity);
            }
        }
        else {
            camera.moveRight(-delta.x() * sensitivity);
            camera.moveUp(delta.y() * sensitivity);
        }
        lastMousePosition = currentMousePosition;
    }
}

void Controller::set3DMode(bool is3D) {
    is3DMode = is3D;
}
