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
    void setCoordinates(std::vector<std::pair<float, float>> coordinates);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    std::vector<std::pair<float, float>> coordinates;
    Controller* controller;
    ObjectLoader objectLoader;

    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;

    void renderPoints();
    bool is3D;
};

#endif // RENDERER_H
