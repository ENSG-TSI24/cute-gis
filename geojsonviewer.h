#ifndef GEOJSONVIEWER_H
#define GEOJSONVIEWER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <nlohmann/json.hpp>
#include <vector>
#include <utility>
#include "camera.h"

#include <QOpenGLFunctions>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <QKeyEvent>

class GeoJsonViewer : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit GeoJsonViewer(QWidget* parent = nullptr);

    // Chargement du fichier GeoJSON
    void loadGeoJSON(const std::string& filePath);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    std::vector<std::pair<float, float>> coordinates;
    Camera camera;
    void renderPoints();
};

#endif // GEOJSONVIEWER_H
