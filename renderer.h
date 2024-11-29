#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include <utility>
#include "controller.h"
#include <QGraphicsSceneMouseEvent>

#include <QMouseEvent>

class Renderer : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit Renderer(QWidget* parent = nullptr);
    void setCoordinates( std::vector<std::pair<float, float>> coordinates);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;


    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;


private:
    std::vector<std::pair<float, float>> coordinates; // Points à dessiner
    Controller* controller; // Pointeur vers Controller

    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;


    void renderPoints();

    // is 3D
    //bool get3D();
};

#endif // RENDERER_H
