#ifndef LAYERTIFF_H
#define LAYERTIFF_H

#include <QGraphicsView>

class LayerRaster
{
public:
    LayerRaster(QImage* _image);
    void renderRasters();
    QImage* image;

};

#endif // LAYERTIFF_H
