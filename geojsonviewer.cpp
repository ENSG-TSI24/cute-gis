#include "geojsonviewer.h"


using json = nlohmann::json;

GeoJsonViewer::GeoJsonViewer(QWidget* parent) : QOpenGLWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
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
    camera.update();
}

void GeoJsonViewer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    camera.update();
    renderPoints();
}



void GeoJsonViewer::keyPressEvent(QKeyEvent *event){
    float step = 10.0;
    switch (event->key()) {
        case( Qt::Key_Up):
            this->camera.moveUp(step);
             break;
        case(Qt::Key_Down):
            this->camera.moveDown(step);
            break;
        case(Qt::Key_Left):
            this->camera.moveLeft(step);
            break;
        case(Qt::Key_Right):
            this->camera.moveRight(step);
            break;
        case(Qt::Key_Z):
            this->camera.moveRight(step);
            break;
        case(Qt::Key_Q):
            this->camera.moveLeft(step);
            break;
        case(Qt::Key_D):
            this->camera.moveRight(step);
            break;
        case(Qt::Key_S):
            this->camera.moveDown(step);
            break;
    }



    camera.update();
    update();
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


