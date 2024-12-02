#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include <utility>
#include <QMatrix4x4>
#include "controller.h"
#include "objectloader.h"

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

    std::vector<std::pair<float, float>> points;
    std::vector<std::vector<std::pair<float, float>>> linestrings;
    std::vector<std::vector<std::vector<std::pair<float, float>>>> polygons;
    ObjectLoader* objectLoader;
    bool is3D;
};

#endif // RENDERER_H
