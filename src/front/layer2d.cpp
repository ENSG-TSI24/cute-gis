#include "layer2d.h"
#include "src/back/session.h"
#include <QOpenGLFunctions>


Layer2d::Layer2d(VectorData data)
{   Session session = Session();
    points = data.GetPoints();
    linestrings = data.GetLineStrings();
    polygons = data.GetPolygons();
    polygons2d  = data.Get2DPolygons();
    calculateBoundingBox();
    attributeHeaders = data.GetAttributName();
    attributes = data.GetAllAttributData();
    name = data.GetPath();
    session.addToJson(data.GetPath());
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
    if (!polygons.empty()){
    for (const auto& polygon : polygons) {
        for (const auto& ring : polygon) {
                        // Dessiner l'intérieur du polygone
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f, 1.0f); // Décalage Z pour éviter les conflits
            glColor3f(1.0f, 0.0f, 0.0f); // Rouge pour le remplissage
            glBegin(GL_POLYGON);
            for (const auto& coord : ring) {
                glVertex3f(std::get<0>(coord), std::get<1>(coord) ,0.0f);
            }
            glEnd();
            glDisable(GL_POLYGON_OFFSET_FILL);

            // Dessiner le contour du polygone
            glColor3f(0.0f, 0.0f, 0.0f); // Noir pour les contours
            glLineWidth(1.0f);
            glEnable(GL_LINE_SMOOTH); // Anti-aliasing pour des lignes lisses
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
            glBegin(GL_LINE_LOOP);
            for (const auto& coord : ring) {
                glVertex3f(std::get<0>(coord), std::get<1>(coord), 0.0f);
            }
            glEnd();
            glDisable(GL_LINE_SMOOTH);
            glDisable(GL_BLEND);
        }
        }
    }else{
        for (const auto& polygon2d : polygons2d) {
        for (const auto& ring : polygon2d) {
            // Dessiner l'intérieur du polygone
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f, 1.0f); // Décalage Z pour éviter les conflits
            glColor3f(1.0f, 0.0f, 0.0f); // Rouge pour le remplissage
            glBegin(GL_POLYGON);
            for (const auto& coord : ring) {
                glVertex3f(coord.first, coord.second, 0.0f);
            }
            glEnd();
            glDisable(GL_POLYGON_OFFSET_FILL);

            // Dessiner le contour du polygone
            glColor3f(0.0f, 0.0f, 0.0f); // Noir pour les contours
            glLineWidth(1.0f);
            glEnable(GL_LINE_SMOOTH); // Anti-aliasing pour des lignes lisses
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
            glBegin(GL_LINE_LOOP);
            for (const auto& coord : ring) {
                glVertex3f(coord.first, coord.second, 0.0f);
            }
            glEnd();
        }
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
    if (!polygons.empty()) {
        for (const auto& polygon : polygons) {
            for (const auto& ring : polygon) {
                for (const auto& coord : ring) {
                    minX = std::min(minX, std::get<0>(coord));
                    maxX = std::max(maxX, std::get<0>(coord));
                    minY = std::min(minY, std::get<1>(coord));
                    maxY = std::max(maxY, std::get<1>(coord));
                }
            }
        }
    } else {
        for (const auto& polygon2d : polygons2d) {
            for (const auto& ring : polygon2d) {
                for (const auto& coord : ring) {
                    minX = std::min(minX, coord.first);
                    maxX = std::max(maxX, coord.first);
                    minY = std::min(minY, coord.second);
                    maxY = std::max(maxY, coord.second);
                }
            }
        }
    }

    // Stocker la bounding box
    this->boundingBox = {minX, maxX, minY, maxY};
    //std::cout<<"min:"<<minX<<"; max:"<<maxX<<"\n";
}

