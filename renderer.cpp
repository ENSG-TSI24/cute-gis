#include "renderer.h"
#include <QMatrix4x4>
#include <QDebug>
#include <QOpenGLFunctions>
#include <glm/vec3.hpp>

Renderer::Renderer(QWidget* parent)
    : QOpenGLWidget(parent), objectLoader(nullptr), geoTiffTextureId(0) {
    controller = new Controller(this);
    setFocusPolicy(Qt::StrongFocus);
    is3D = false;
}

Renderer::~Renderer() {
    if (objectLoader) {
        delete objectLoader;
    }
    delete controller;

    // Libérer la texture GeoTIFF si elle a été créée
    if (geoTiffTextureId != 0) {
        glDeleteTextures(1, &geoTiffTextureId);
    }
}

void Renderer::keyPressEvent(QKeyEvent* event) {
    this->controller->ControllerkeyPressEvent(event);
}

void Renderer::wheelEvent(QWheelEvent* event) {
    this->controller->ControllerwheelEvent(event);
}

void Renderer::mousePressEvent(QMouseEvent* event) {
    controller->ControllerMousePressEvent(event);
}

void Renderer::mouseReleaseEvent(QMouseEvent* event) {
    controller->ControllerMouseReleaseEvent(event);
}

void Renderer::mouseMoveEvent(QMouseEvent* event) {
    controller->ControllerMouseMoveEvent(event);
    update(); // Redessine la scène après un déplacement
}


void Renderer::initializeGL() {


    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    GLint maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    qDebug() << "Maximum texture size supported by OpenGL:" << maxTextureSize;
}



void Renderer::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);

    controller->getCamera().update();

    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(45.0f, float(w) / float(h), 0.1f, 100.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projectionMatrix.constData());
}

void Renderer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!objectLoader) {
        controller->getCamera().update();
        renderPoints();
        renderLinestrings();
        renderPolygons();
        renderGeoTiff(); // Appeler le rendu GeoTIFF ici
    } else if (objectLoader) {
        QMatrix4x4 viewMatrix;

        QVector3D cameraPosition(controller->getCamera().getX(),
                                 controller->getCamera().getY(),
                                 controller->getCamera().getZoom());
        QVector3D target(0.0f, 0.0f, 0.0f);
        QVector3D upVector(0.0f, 1.0f, 0.0f);
        viewMatrix.lookAt(cameraPosition, target, upVector);

        QMatrix4x4 modelMatrix;
        modelMatrix.translate(0.0f, 0.0f, -3.0f);
        modelMatrix.rotate(objectLoader->getAngle(), 0.0f, 1.0f, 0.0f);
        modelMatrix.scale(0.005f);

        QMatrix4x4 modelViewMatrix = viewMatrix * modelMatrix;

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(modelViewMatrix.constData());

        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_TRIANGLES);

        const auto& vertices = objectLoader->getVertices();
        for (const auto& vertex : vertices) {
            glVertex3f(vertex.x, vertex.y, vertex.z);
        }

        glEnd();
    } else {
        qWarning() << "No ObjectLoader assigned for 3D rendering.";
    }
}

void Renderer::renderPoints() {
    glColor3f(0.0f, 0.0f, 1.0f); // Couleur bleue
    glPointSize(5.0f);

    glBegin(GL_POINTS);
    for (const auto& coord : points) {
        glVertex2f(coord.first, coord.second);
    }
    glEnd();
}

void Renderer::renderLinestrings() {
    glColor3f(0.0f, 1.0f, 0.0f);
    glLineWidth(2.0f);
    for (const auto& line : linestrings) {
        glBegin(GL_LINE_STRIP);
        for (const auto& coord : line) {
            glVertex2f(coord.first, coord.second);
        }
        glEnd();
    }
}

void Renderer::renderPolygons() {
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

void Renderer::renderGeoTiff() {
    if (geoTiffTextureId == 0) return; // Rien à rendre si la texture n'existe pas

    // Accès explicite aux membres de boundingBox
    double xMin = boundingBox.minX;
    double xMax = boundingBox.maxX;
    double yMin = boundingBox.minY;
    double yMax = boundingBox.maxY;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, geoTiffTextureId);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(xMin, yMin, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(xMax, yMin, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(xMax, yMax, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(xMin, yMax, 0.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void Renderer::reset() {
    points.clear();
    linestrings.clear();
    polygons.clear();

    if (objectLoader) {
        delete objectLoader;
        objectLoader = nullptr;
    }

    if (geoTiffTextureId != 0) {
        glDeleteTextures(1, &geoTiffTextureId);
        geoTiffTextureId = 0;
    }

    is3D = false;
    update(); // Redessine la scène après réinitialisation
}

void Renderer::setPoints(std::vector<std::pair<float, float>> points) {
    this->points = std::move(points); // Évite une copie inutile
}

void Renderer::setLinestrings(std::vector<std::vector<std::pair<float, float>>> linestrings) {
    this->linestrings = std::move(linestrings); // Évite une copie inutile
}

void Renderer::setPolygons(std::vector<std::vector<std::vector<std::pair<float, float>>>> polygons) {
    this->polygons = std::move(polygons); // Évite une copie inutile
}

void Renderer::calculateBoundingBox() {
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
    boundingBox = {minX, maxX, minY, maxY};
    std::cout << "BoundingBox - minX:" << minX << ", maxX:" << maxX
              << ", minY:" << minY << ", maxY:" << maxY << std::endl;
}

void Renderer::setObjectLoader(ObjectLoader* loader) {
    if (objectLoader) {
        delete objectLoader; // Supprime l'ancien loader
    }
    objectLoader = loader; // Attribue le nouveau loader
    update(); // Redessine la scène
}

void Renderer::setIs3D(bool enabled) {
    is3D = enabled;
    update(); // Redessine la scène avec les nouvelles configurations
}

void Renderer::loadGeoTiff(const std::string& filePath) {
    if (!geoTiffLoader.loadGeoTiff(filePath)) {
        throw std::runtime_error("Failed to load GeoTIFF file: " + filePath);
    }

    if (geoTiffTextureId != 0) {
        glDeleteTextures(1, &geoTiffTextureId);
    }

    textureCreator = new TextureGeoTiff();
    geoTiffTextureId = textureCreator->createTextureFromRaster(
        geoTiffLoader.getRasterData(),
        geoTiffLoader.getWidth(),
        geoTiffLoader.getHeight()
    );

    calculateBoundingBox();
    update(); // Redessine la scène
}


