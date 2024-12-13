#include "layer2d.h"
#include <QOpenGLFunctions>


Layer2d::Layer2d(VectorData data)
{
    points = data.GetPoints();
    linestrings = data.GetLineStrings();
    polygons = data.GetPolygons();
    polygons2d  = data.Get2DPolygons();
    boundingBox = calculateBoundingBox();
    attributeHeaders = data.GetAttributName();
    attributes = data.GetAllAttributData();
}

Layer2d::~Layer2d(){}

void Layer2d::render() {
    renderPolygons();
    renderLinestrings();
    renderPoints();
}

void Layer2d::renderPoints() {
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (size_t i = 0; i < points.size(); ++i) {
        if (static_cast<int>(i) == highlightedIndex) {
            glColor4f(1.0f, 1.0f, 0.0f, opacity); // Yellow for highlight
        } else {
            glColor4f(0.0f, 0.0f, 1.0f, opacity); // Blue for normal points
        }
        glVertex3f(points[i].first, points[i].second, 0.0f);
    }

    glEnd();
}


void Layer2d::renderLinestrings() {
    for (size_t i = 0; i < linestrings.size(); ++i) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (static_cast<int>(i) == highlightedIndex) {
            // Highlighted linestring
            glLineWidth(5.0f);
            glColor4f(1.0f, 1.0f, 0.0f, opacity); // Yellow for highlight
        } else {
            // Normal linestring
            glLineWidth(2.0f);
            glColor4f(0.0f, 1.0f, 0.0f, opacity); // Green for normal
        }

        glBegin(GL_LINE_STRIP);
        for (const auto& coord : linestrings[i]) {
            glVertex3f(coord.first, coord.second, 0.0f);
        }
        glEnd();
    }
    // Reset line width
    glLineWidth(1.0f);
}


void Layer2d::renderPolygons() {
    glLineWidth(1.0f);

    if (!polygons.empty()) {
        for (size_t i = 0; i < polygons.size(); ++i) {
            const auto& polygon = polygons[i];

            for (const auto& ring : polygon) {

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                // Highlight logic
                if (static_cast<int>(i) == highlightedIndex) {
                    glEnable(GL_POLYGON_OFFSET_FILL);
                    glPolygonOffset(1.0f, 1.0f); // Décalage Z pour éviter les conflits
                    glColor4f(1.0f, 1.0f, 0.0f, opacity); // Yellow for highlight
                    glBegin(GL_POLYGON);
                    for (const auto& coord : ring) {
                        glVertex3f(std::get<0>(coord), std::get<1>(coord), 0.0f);
                    }
                    glEnd();
                    glDisable(GL_POLYGON_OFFSET_FILL);

                    glColor4f(1.0f, 1.0f, 0.0f, opacity); // Yellow for the contour
                    glLineWidth(3.0f);
                } else {
                    // Normal polygon fill
                    glEnable(GL_POLYGON_OFFSET_FILL);
                    glPolygonOffset(1.0f, 1.0f); // Décalage Z pour éviter les conflits
                    glColor4f(1.0f, 0.0f, 0.0f, opacity); // Rouge pour le remplissage
                    glBegin(GL_POLYGON);
                    for (const auto& coord : ring) {
                        glVertex3f(std::get<0>(coord), std::get<1>(coord), 0.0f);
                    }
                    glEnd();
                    glDisable(GL_POLYGON_OFFSET_FILL);

                    glColor4f(0.0f, 0.0f, 0.0f, opacity); // Noir pour les contours
                    glLineWidth(1.0f);
                }

                // Draw the contour of the polygon
                glEnable(GL_LINE_SMOOTH); // Anti-aliasing for smooth lines
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
    } else {
        for (size_t i = 0; i < polygons2d.size(); ++i) {
            const auto& polygon2d = polygons2d[i];

            for (const auto& ring : polygon2d) {
                // Highlight logic
                if (static_cast<int>(i) == highlightedIndex) {
                    glEnable(GL_POLYGON_OFFSET_FILL);
                    glPolygonOffset(1.0f, 1.0f); // Décalage Z pour éviter les conflits
                    glColor4f(1.0f, 1.0f, 0.0f, opacity); // Yellow for highlight
                    glBegin(GL_POLYGON);
                    for (const auto& coord : ring) {
                        glVertex3f(coord.first, coord.second, 0.0f);
                    }
                    glEnd();
                    glDisable(GL_POLYGON_OFFSET_FILL);

                    glColor4f(1.0f, 1.0f, 0.0f, opacity); // Yellow for the contour
                    glLineWidth(3.0f);
                } else {
                    // Normal polygon fill
                    glEnable(GL_POLYGON_OFFSET_FILL);
                    glPolygonOffset(1.0f, 1.0f); // Décalage Z pour éviter les conflits
                    glColor4f(1.0f, 0.0f, 0.0f, opacity); // Rouge pour le remplissage
                    glBegin(GL_POLYGON);
                    for (const auto& coord : ring) {
                        glVertex3f(coord.first, coord.second, 0.0f);
                    }
                    glEnd();
                    glDisable(GL_POLYGON_OFFSET_FILL);

                    glColor4f(0.0f, 0.0f, 0.0f, opacity); // Noir pour les contours
                    glLineWidth(1.0f);
                }

                // Draw the contour of the polygon
                glEnable(GL_LINE_SMOOTH); // Anti-aliasing for smooth lines
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
                glBegin(GL_LINE_LOOP);
                for (const auto& coord : ring) {
                    glVertex3f(coord.first, coord.second, 0.0f);
                }
                glEnd();
                glDisable(GL_LINE_SMOOTH);
                glDisable(GL_BLEND);
            }
        }
    }
}




BoundingBox Layer2d::calculateBoundingBox() {
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
    return BoundingBox(minX, maxX, minY, maxY);
    //std::cout<<"min:"<<minX<<"; max:"<<maxX<<"\n";
}




void Layer2d::highlightGeometry(int rowIndex) {
    if (rowIndex >= 0 && rowIndex < attributes.size()) {
        highlightedIndex = rowIndex; // Store the highlighted feature index
    } else {
        highlightedIndex = -1; // Clear highlight if the index is invalid
    }

    glColor4f(1.0f, 1.0f, 0.0f, opacity); // Highlight color (yellow)

    // Highlight point
    if (!points.empty() && rowIndex < points.size()) {
        glPointSize(10.0f);
        glBegin(GL_POINTS);
        glVertex3f(points[rowIndex].first, points[rowIndex].second, 0.0f);
        glEnd();
    }

    // Highlight linestring
    if (!linestrings.empty() && rowIndex < linestrings.size()) {
        glLineWidth(5.0f);
        glBegin(GL_LINE_STRIP);
        for (const auto& coord : linestrings[rowIndex]) {
            glVertex3f(coord.first, coord.second, 0.0f);
        }
        glEnd();
    }

    // Highlight polygon
    if (!polygons.empty() && rowIndex < polygons.size()) {
        glLineWidth(5.0f);
        for (const auto& ring : polygons[rowIndex]) {
            glBegin(GL_LINE_LOOP);
            for (const auto& coord : ring) {
                glVertex3f(std::get<0>(coord), std::get<1>(coord), 0.0f);
            }
            glEnd();
        }
    }
}
std::string Layer2d::getName() {
    return name;
}

void Layer2d::setName(std::string _name) {
    name = _name;
}

bool Layer2d::getIsVisible() {
    return isVisible;
}

void Layer2d::setIsVisible(bool _isVisible) {
    isVisible = _isVisible;
}

BoundingBox Layer2d::getBoundingBox() {
    return boundingBox;
}

