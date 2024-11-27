#include "geojsonviewer.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <QKeyEvent>
#include <QWheelEvent>

using json = nlohmann::json;

GeoJsonViewer::GeoJsonViewer(QWidget* parent)
    : QOpenGLWidget(parent) {
    setFocusPolicy(Qt::StrongFocus); // Active le focus pour recevoir les événements clavier
}

void GeoJsonViewer::loadGeoJSON(const std::string& filePath) {
    // Charger le fichier GeoJSON
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
    lineStrings.clear();
    polygons.clear();

    if (geojson["type"] != "FeatureCollection") {
        throw std::runtime_error("Le fichier GeoJSON n'est pas de type FeatureCollection.");
    }

    for (const auto& feature : geojson["features"]) {
        if (!feature.contains("geometry")) continue;

        auto geometry = feature["geometry"];
        const std::string type = geometry["type"];

        if (type == "Point" && geometry.contains("coordinates")) {
            const auto& coord = geometry["coordinates"];
            if (coord.size() >= 2) {
                coordinates.emplace_back(coord[0], coord[1]);
            }
        } else if (type == "LineString" && geometry.contains("coordinates")) {
            std::vector<std::pair<float, float>> line;
            for (const auto& coord : geometry["coordinates"]) {
                if (coord.size() >= 2) {
                    line.emplace_back(coord[0], coord[1]);
                }
            }
            if (!line.empty()) {
                lineStrings.push_back(line);
            }
        } else if (type == "Polygon" && geometry.contains("coordinates")) {
            std::vector<std::vector<std::pair<float, float>>> polygon;
            for (const auto& ring : geometry["coordinates"]) {
                std::vector<std::pair<float, float>> ringPoints;
                for (const auto& coord : ring) {
                    if (coord.size() >= 2) {
                        ringPoints.emplace_back(coord[0], coord[1]);
                    }
                }
                if (!ringPoints.empty()) {
                    polygon.push_back(ringPoints);
                }
            }
            if (!polygon.empty()) {
                polygons.push_back(polygon);
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
    camera.apply(); // Applique les transformations de la caméra
}

void GeoJsonViewer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    camera.apply(); // Applique les transformations de la caméra

    renderPoints();      // Dessine les points
    renderLineStrings(); // Dessine les lignes
    renderPolygons();    // Dessine les polygones
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

void GeoJsonViewer::renderLineStrings() {
    glColor3f(0.0f, 1.0f, 0.0f); // Vert
    glLineWidth(2.0f);
    for (const auto& line : lineStrings) {
        glBegin(GL_LINE_STRIP);
        for (const auto& coord : line) {
            glVertex2f(coord.first, coord.second);
        }
        glEnd();
    }
}

void GeoJsonViewer::renderPolygons() {
    glColor3f(1.0f, 0.0f, 0.0f); // Rouge
    glLineWidth(1.0f);
    for (const auto& polygon : polygons) {
        for (const auto& ring : polygon) {
            glBegin(GL_LINE_LOOP);
            for (const auto& coord : ring) {
                glVertex2f(coord.first, coord.second);
            }
            glEnd();
        }
    }
}



// Événement pour les touches directionnelles
void GeoJsonViewer::keyPressEvent(QKeyEvent* event) {
    const float step = 10.0f; // Pas de déplacement
    switch (event->key()) {
    case Qt::Key_Up:
        camera.moveUp(step);
        break;
    case Qt::Key_Down:
        camera.moveDown(step);
        break;
    case Qt::Key_Left:
        camera.moveLeft(step);
        break;
    case Qt::Key_Right:
        camera.moveRight(step);
        break;
    default:
        QOpenGLWidget::keyPressEvent(event); // Propagation de l'événement
        return;
    }
    update(); // Redessine l'écran après un déplacement
}

// Événement de la molette pour zoomer
void GeoJsonViewer::wheelEvent(QWheelEvent* event) {
    if (event->angleDelta().y() > 0) {
        camera.setZoomLevel(camera.getZoomLevel() * 1.05f); // Zoom avant
    } else {
        camera.setZoomLevel(camera.getZoomLevel() / 1.05f); // Zoom arrière
    }
    update(); // Redessine l'écran après un zoom
}
