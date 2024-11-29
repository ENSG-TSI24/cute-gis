#include "controller.h"

Controller::Controller(QWidget* parent)
    : QWidget(parent), camera() {
}

Camera& Controller::getCamera() {
    return camera; // Retourne la référence à la caméra
}

void Controller::ControllerwheelEvent(QWheelEvent* event) {
    std::cout <<"la vie cette sale race\n";
    float zoomStep = 1.0f;
    if (event->angleDelta().y() > 0) {
        camera.setZoom(zoomStep);
    } else if (event->angleDelta().y() < 0) {
        camera.setZoom(-zoomStep);
    }

    camera.update();
    update();
}


void Controller::ControllerkeyPressEvent(QKeyEvent *event){
    float step = 10.0;
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

    camera.update();
    update();
}


