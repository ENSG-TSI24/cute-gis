#include "geojsonviewer.h"
#include <fstream>
#include <stdexcept>
#include <iostream>

using json = nlohmann::json;

GeoJsonViewer::GeoJsonViewer(QWidget* parent) : QOpenGLWidget(parent) {}

void GeoJsonViewer::loadGeoJSON(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier GeoJSON : " + filePath);
    }

    json geojson;
    try {
        file >> geojson;
    } catch (const std::exception& e) {
        throw std::runtime_error("Erreur lors du parsing du fichier GeoJSON : " + std::string(e.what()));
    }

    coordinates.clear();
    if (geojson["type"] != "FeatureCollection") {
        throw std::runtime_error("Le fichier GeoJSON n'est pas de type FeatureCollection.");
    }

    for (const auto& feature : geojson["features"]) {
        if (!feature.contains("geometry")) continue;

        auto geometry = feature["geometry"];
        if (geometry["type"] == "Point" && geometry.contains("coordinates")) {
            const auto& coord = geometry["coordinates"];
            if (coord.size() >= 2) {
                coordinates.emplace_back(coord[0], coord[1]); // (longitude, latitude)
            }
        }
    }
}

void GeoJsonViewer::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Fond blanc
}

void GeoJsonViewer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    updateCamera();
}

void GeoJsonViewer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    updateCamera();
    renderPoints();
}

void GeoJsonViewer::renderPoints() {
    glColor3f(0.0f, 0.0f, 1.0f); // Bleu
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for (const auto& coord : coordinates) {
        glVertex2f(coord.first, coord.second);
    }
    glEnd();
}

void GeoJsonViewer::updateCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float left = -180.0f / zoomLevel + cameraX;
    float right = 180.0f / zoomLevel + cameraX;
    float bottom = -90.0f / zoomLevel + cameraY;
    float top = 90.0f / zoomLevel + cameraY;
    glOrtho(left, right, bottom, top, -1.0f, 1.0f);
}
