#include "layertiff.h"
#include "geotiffloader.h"

#include <QOpenGLFunctions>
#include <iostream>


LayerTiff::LayerTiff(QImage* _image)
    : image(_image){}


void LayerTiff::renderTiff() {

    QImage glImage = this->image->convertToFormat(QImage::Format_RGBA8888);
    int width = glImage.width();
    int height = glImage.height();
    const unsigned char* data = glImage.bits();

    glPixelZoom(1.0f, 1.0f);
    glRasterPos2i(-1, -1);

    glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
}
