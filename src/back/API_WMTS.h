
#ifndef WMTSDATA_H
#define WMTSDATA_H

#include <gdal_priv.h>
#include <string>
#include <vector>
#include <ogrsf_frmts.h>
#include <QImage>
#include <QGraphicsScene>
#include "DataProvider.h"

class API_WMTS : public DataProvider {
public:

    API_WMTS(const char* link);
    void loadDataset();

    // Prototype foireux de chargement et affichage des tuiles ( attendre que le front est fini la gestion zoom/deplacement )
    QImage getTileAsImage(const char* layerName, int zoom, int row, int col);
    void displayTiles(const char* layerName, int zoom, int centerRow, int centerCol, int gridSize, QGraphicsScene* scene);

protected:
    const char* url;
};

#endif // WMTSDATA_H
