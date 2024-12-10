#include "layer2d.h"
#include <QOpenGLFunctions>


Layer2d::Layer2d(Geojsonloader data)
{
    points = data.getPoints();
    linestrings = data.getLinestrings();
    polygons = data.getPolygons();
    calculateBoundingBox();
}


Layer2d::~Layer2d(){

}

void Layer2d::renderPoints() {
    glColor3f(0.0f, 0.0f, 1.0f); // Couleur bleue
    glPointSize(5.0f);

    glBegin(GL_POINTS);
    for (const auto& coord : points) {
        glVertex3f(coord.first, coord.second, 0.0f);
    }
    glEnd();
}

void Layer2d::renderLinestrings() {
    glColor3f(0.0f, 1.0f, 0.0f);
    glLineWidth(2.0f);
    for (const auto& line : linestrings) {
        glBegin(GL_LINE_STRIP);
        for (const auto& coord : line) {
            glVertex3f(coord.first, coord.second, 0.0f);
        }
        glEnd();
    }
}

void Layer2d::renderPolygons() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(1.0f);
    for (const auto& polygon : polygons) {
        for (const auto& ring : polygon) {
            glBegin(GL_LINE_LOOP);
            for (const auto& coord : ring) {
                glVertex3f(coord.first, coord.second,0.0f);
            }
            glEnd();
        }
    }
}

void Layer2d::calculateBoundingBox() {
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();

    // Inclure les points
    for (const auto& point : points) {
        minX = std::min(minX, point.first);
        maxX = std::max(maxX, point.first);
        minY = std::min(minY, point.second);
        maxY = std::max(maxY, point.second);
    }

    // Inclure les LineStrings
    for (const auto& line : linestrings) {
        for (const auto& coord : line) {
            minX = std::min(minX, coord.first);
            maxX = std::max(maxX, coord.first);
            minY = std::min(minY, coord.second);
            maxY = std::max(maxY, coord.second);
        }
    }

    // Inclure les Polygons
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

    // Stocker la bounding box
    this->boundingBox = {minX, maxX, minY, maxY};
    //std::cout<<"min:"<<minX<<"; max:"<<maxX<<"\n";
}

