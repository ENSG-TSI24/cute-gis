#ifndef GEOJSONVIEWER_H
#define GEOJSONVIEWER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <nlohmann/json.hpp>
#include <vector>
#include <utility>

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

private:
    std::vector<std::pair<float, float>> coordinates;
    float cameraX = 0.0f, cameraY = 0.0f; // Position de la caméra
    float zoomLevel = 1.0f;               // Niveau de zoom

    void renderPoints();
    void updateCamera();
};

#endif // GEOJSONVIEWER_H
