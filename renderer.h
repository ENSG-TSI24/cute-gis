#ifndef RENDERER_H
#define RENDERER_H

//#include <GL/glew.h> // GLEW en premier

#include "texturegeotif.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include <utility>
#include <QMatrix4x4>
#include "controller.h"
#include "objectloader.h"
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include "geotifloader.h"
#include "geotifloader.h"


class Renderer : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit Renderer(QWidget* parent = nullptr);
    virtual ~Renderer();

    void setPoints(std::vector<std::pair<float, float>> points);
    void setLinestrings(std::vector<std::vector<std::pair<float, float>>> linestrings);
    void setPolygons(std::vector<std::vector<std::vector<std::pair<float, float>>>> polygons);

    void setIs3D(bool enabled);

    void setObjectLoader(ObjectLoader* loader);
    void reset();
    void calculateBoundingBox();
    BoundingBox boundingBox;
    Controller* controller;


    void loadGeoTiff(const std::string& filePath); // Charger une image GeoTIFF

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void renderPoints();
    void renderLinestrings();
    void renderPolygons();

    void renderGeoTiff(); // Rendu GeoTIFF


    std::vector<std::pair<float, float>> points;
    std::vector<std::vector<std::pair<float, float>>> linestrings;
    std::vector<std::vector<std::vector<std::pair<float, float>>>> polygons;
    ObjectLoader* objectLoader;
    bool is3D;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

    GLuint geoTiffTextureId = 0; // ID de la texture GeoTIFF
    TextureGeoTiff* textureCreator = nullptr; // Pour créer la texture OpenGL
    GeoTiffLoader geoTiffLoader; // Instance de GeoTiffLoader

};

#endif // RENDERER_H
