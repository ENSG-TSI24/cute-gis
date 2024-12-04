#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include <utility>
#include <QMatrix4x4>
#include "controller.h"
#include "objectloader.h"
#include <QGraphicsSceneMouseEvent>
#include "layer2d.h"

#include <QMouseEvent>

class Renderer : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit Renderer(QWidget* parent = nullptr);
    virtual ~Renderer();

    void setIs3D(bool enabled);

    void setObjectLoader(ObjectLoader* loader);
    void reset();
    void calculateBoundingBox();
    BoundingBox boundingBox;
    Controller* controller;

    std::vector<Layer2d> lst_layers2d;



protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:

    void renderLayers2d();

    ObjectLoader* objectLoader;
    bool is3D;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;


    // is 3D
    //bool get3D();
};

#endif // RENDERER_H
