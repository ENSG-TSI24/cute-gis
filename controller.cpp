#include "controller.h"

Controller::Controller(QWidget* parent)
    : QWidget(parent), camera(), isDragging(false) {}

//Controller::Controller(QWidget* parent)
//    : QWidget(parent), camera() {
//}

Camera& Controller::getCamera() {
    return camera; // Retourne la référence à la caméra
}

/**void Controller::ControllerQMouseEvent(QMouseEvent *event){

    if(event->button()==Qt::LeftButton){
         std::cout << "oui \n" <<std::endl;
    }
    if(event->button()!=Qt::LeftButton ){
        std::cout << "non \n" <<std::endl;
    }

}
**/


void Controller::ControllerwheelEvent(QWheelEvent* event) {
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
        float sensitivity = 0.25;
        QPoint currentMousePosition = event->pos();
        QPoint delta = currentMousePosition - lastMousePosition;
        camera.moveRight(-delta.x() * sensitivity);
        camera.moveUp(delta.y() * sensitivity);

        lastMousePosition = currentMousePosition;
    }
}


