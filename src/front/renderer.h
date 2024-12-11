#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include <utility>
#include <QMatrix4x4>
#include "controller.h"
#include <QGraphicsSceneMouseEvent>

#include <QMouseEvent>

class Renderer2D;
class Renderer3D;

class Renderer : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit Renderer(QWidget* parent = nullptr);
    virtual ~Renderer();

    void reset();
    void reset2D();
    void reset3D();
    void setIs3D(bool enabled);
    Renderer2D* getRenderer2d();
    Renderer3D* getRenderer3d();


    //void reset();

    bool getIs3D();
    void calculateBoundingBox();
    BoundingBox boundingBox;
    Controller* controller;


protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    bool is3D;
    Renderer2D* renderer2d;
    Renderer3D* renderer3d;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;


    // is 3D
    //bool get3D();
};

#endif // RENDERER_H
