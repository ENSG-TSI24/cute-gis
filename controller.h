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

protected:
    void wheelEvent(QWheelEvent* event) override; // Gère les événements molette
    void keyPressEvent(QKeyEvent* event) override; // Gère les événements clavier

private:
    Camera camera; // Caméra contrôlée par Controller
};

#endif // CONTROLLER_H
