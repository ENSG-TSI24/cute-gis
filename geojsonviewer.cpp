#include "geojsonviewer.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <QKeyEvent>
#include <QWheelEvent>

using json = nlohmann::json;

GeoJsonViewer::GeoJsonViewer(QWidget* parent)
    : QOpenGLWidget(parent), minX(std::numeric_limits<float>::max()),
      maxX(std::numeric_limits<float>::lowest()),
      minY(std::numeric_limits<float>::max()),
      maxY(std::numeric_limits<float>::lowest()) {
    setFocusPolicy(Qt::StrongFocus); // Active le focus pour recevoir les événements clavier
}

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

    // Normaliser les coordonnées après le chargement
    normalizeCoordinates();
}

void GeoJsonViewer::normalizeCoordinates() {
    // Calculer les min/max des coordonnées
    for (const auto& coord : coordinates) {
        minX = std::min(minX, coord.first);
        maxX = std::max(maxX, coord.first);
        minY = std::min(minY, coord.second);
        maxY = std::max(maxY, coord.second);
    }
    for (const auto& line : lineStrings) {
        for (const auto& coord : line) {
            minX = std::min(minX, coord.first);
            maxX = std::max(maxX, coord.first);
            minY = std::min(minY, coord.second);
            maxY = std::max(maxY, coord.second);
        }
    }
    for (const auto& polygon : polygons) {
        for (const auto& ring : polygon) {
            for (const auto& coord : ring) {
                minX = std::min(minX, coord.first);
                maxX = std::max(maxX, coord.first);
                minY = std::min(minY, coord.second);
                maxY = std::max(maxY, coord.second);
            }
        }
    }

    // Normaliser toutes les coordonnées dans [-1, 1]
    auto normalize = [this](float value, float min, float max) {
        return (2.0f * (value - min) / (max - min)) - 1.0f;
    };

    for (auto& coord : coordinates) {
        coord.first = normalize(coord.first, minX, maxX);
        coord.second = normalize(coord.second, minY, maxY);
    }
    for (auto& line : lineStrings) {
        for (auto& coord : line) {
            coord.first = normalize(coord.first, minX, maxX);
            coord.second = normalize(coord.second, minY, maxY);
        }
    }
    for (auto& polygon : polygons) {
        for (auto& ring : polygon) {
            for (auto& coord : ring) {
                coord.first = normalize(coord.first, minX, maxX);
                coord.second = normalize(coord.second, minY, maxY);
            }
        }
    }
}

void GeoJsonViewer::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void GeoJsonViewer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    camera.apply();
}

void GeoJsonViewer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    camera.apply();

    renderPoints();
    renderLineStrings();
    renderPolygons();
}

void GeoJsonViewer::renderPoints() {
    glColor3f(0.0f, 0.0f, 1.0f);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for (const auto& coord : coordinates) {
        glVertex2f(coord.first, coord.second);
    }
    glEnd();
}

void GeoJsonViewer::renderLineStrings() {
    glColor3f(0.0f, 1.0f, 0.0f);
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
    glColor3f(1.0f, 0.0f, 0.0f);
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

void GeoJsonViewer::keyPressEvent(QKeyEvent* event) {
    const float step = 10.0f;
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
        QOpenGLWidget::keyPressEvent(event);
        return;
    }
    update();
}

void GeoJsonViewer::wheelEvent(QWheelEvent* event) {
    if (event->angleDelta().y() > 0) {
        camera.setZoomLevel(camera.getZoomLevel() * 1.05f);
    } else {
        camera.setZoomLevel(camera.getZoomLevel() / 1.05f);
    }
    update();
}
