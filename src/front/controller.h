#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QWidget>
#include <QKeyEvent>
#include <QWheelEvent>
#include "camera.h"

class Controller : public QWidget {
    Q_OBJECT

public:
    explicit Controller(QWidget* parent = nullptr);

    Camera& getCamera(); // Fournit un accès à la caméra
    void set3DMode(bool is3D);
    void ControllerwheelEvent(QWheelEvent* event) ; // Gère les événements molette
    void ControllerkeyPressEvent(QKeyEvent* event) ; // Gère les événements clavier
    void ControllerQMouseEvent(QMouseEvent *event);

    void ControllerMousePressEvent(QMouseEvent* event); // Début du drag
    void ControllerMouseReleaseEvent(QMouseEvent* event); // Fin du drag
    void ControllerMouseMoveEvent(QMouseEvent* event); // Déplacement pendant le drag

    bool get3DMode();

private:
    Camera camera; // Caméra contrôlée par Controller
    bool isDragging;
    QPoint lastMousePosition;
    bool is3DMode;

};

#endif // CONTROLLER_H
