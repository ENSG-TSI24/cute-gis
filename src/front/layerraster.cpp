#include "layerraster.h"
#include "geotiffloader.h"

#include <QOpenGLFunctions>
#include <iostream>


LayerRaster::LayerRaster(RasterData raster)
{
    image = raster.GetImage();
    topLeft = raster.GetTopLeftGeoCoordinates();
    bottomRight = raster.GetBottomRightGeoCoordinates();
    boundingBox = calculateBoundingBox();
}

LayerRaster::~LayerRaster(){}

void LayerRaster::renderRasters() {

    QImage glImage = image.convertToFormat(QImage::Format_RGBA8888);
    int width = glImage.width();
    int height = glImage.height();

    // Calcul des coins géographiques
    double xMin = topLeft.first;
    double yMax = topLeft.second;
    double xMax = bottomRight.first;
    double yMin = bottomRight.second;

    // Charger l'image comme texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, glImage.bits());

    // Activer les textures
    glEnable(GL_TEXTURE_2D);

    glColor3f(1.0f, 1.0f, 1.0f);
    // Dessiner le quadrilatère texturé
    glBegin(GL_QUADS);
        glTexCoord3f(0.0f, 0.0f, 0.0f); glVertex3f(xMin, yMin, 0.0); // Bas gauche
        glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(xMax, yMin, 0.0); // Bas droite
        glTexCoord3f(1.0f, 1.0f, 0.0f); glVertex3f(xMax, yMax, 0.0); // Haut droite
        glTexCoord3f(0.0f, 1.0f, 0.0f); glVertex3f(xMin, yMax, 0.0); // Haut gauche
    glEnd();

    // Désactiver les textures
    glDisable(GL_TEXTURE_2D);

    // Nettoyer la texture
    glDeleteTextures(1, &textureID);


}

BoundingBox LayerRaster::calculateBoundingBox() {

    // Calcul des coordonnées géographiques basées sur les métadonnées
    float minX = topLeft.first;
    float maxY = topLeft.second;
    float maxX = bottomRight.first;
    float minY = bottomRight.second;

    // Assignez les coordonnées à la bounding box
    return BoundingBox(minX, maxX, minY, maxY);
}
