#ifndef LAYERTIFF_H
#define LAYERTIFF_H

#include <QGraphicsView>

class LayerTiff
{
public:
    LayerTiff(QImage* _image);
    void renderTiff();
    QImage* image;

};

#endif // LAYERTIFF_H
