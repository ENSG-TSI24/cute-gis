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

    void setPoints(std::vector<std::pair<float, float>> points);
    void setLinestrings(std::vector<std::vector<std::pair<float, float>>> linestrings);
    void setPolygons(std::vector<std::vector<std::vector<std::pair<float, float>>>> polygons);

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
    void renderPoints();
    void renderLinestrings();
    void renderPolygons();
    void renderLayers2d();
    void paintGl3D();
    void paintGl2D();

    std::vector<std::pair<float, float>> points;
    std::vector<std::vector<std::pair<float, float>>> linestrings;
    std::vector<std::vector<std::vector<std::pair<float, float>>>> polygons;
    ObjectLoader* objectLoader;
    bool is3D;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;


    // is 3D
    //bool get3D();
};

#endif // RENDERER_H
