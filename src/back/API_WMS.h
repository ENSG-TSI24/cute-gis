#ifndef API_WMS_H
#define API_WMS_H

#include <gdal_priv.h>
#include <string>
#include <vector>
#include <ogrsf_frmts.h>
#include "DataProvider.h"

class API_WMS: public DataProvider {
public:
    API_WMS(const char* link) ;

    void loadDataset();

    // Télécharge une tuile spécifique et la convertit en GeoTIFF
    void downloadTileToGeoTiff(const std::string& layerName, int zoom, int row, int col);

    // Charge une grille de tuiles autour d'une position donnée et les convertit en GeoTIFF
    void loadTileGridToGeoTiff(const char* layerName, int zoom, int centerRow, int centerCol, int gridSize, const char* outputPrefix);
    const char* getOutput();

protected:
    const char* url;
    const char* output_path ;
};

#endif // API_WMS_H