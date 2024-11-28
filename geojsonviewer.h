#ifndef GEOJSONVIEWER_H
#define GEOJSONVIEWER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include <utility>
#include <string>
#include <limits>
#include "camera.h"

class GeoJsonViewer : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit GeoJsonViewer(QWidget* parent = nullptr);

    void loadGeoJSON(const std::string& filePath);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    std::vector<std::pair<float, float>> coordinates; // Points GeoJSON
    std::vector<std::vector<std::pair<float, float>>> lineStrings; // LineStrings
    std::vector<std::vector<std::vector<std::pair<float, float>>>> polygons; // Polygons

    Camera camera;

    // Normalisation des coordonnées
    float minX, maxX, minY, maxY;
    void normalizeCoordinates();

    void renderPoints();
    void renderLineStrings();
    void renderPolygons();
};

#endif // GEOJSONVIEWER_H
