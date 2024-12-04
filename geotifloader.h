#ifndef GEOTIFFLOADER_H
#define GEOTIFFLOADER_H


#include <string>
#include <vector>
#include <gdal_priv.h>

class GeoTiffLoader {
public:
    GeoTiffLoader();
    ~GeoTiffLoader();

    bool loadGeoTiff(const std::string& filePath); // Charge un GeoTIFF
    bool isValid() const; // Vérifie si les données sont valides
    const std::vector<unsigned char>& getRasterData() const; // Accès aux données raster
    int getWidth() const; // Largeur de l'image
    int getHeight() const; // Hauteur de l'image

private:
    GDALDataset* dataset;
    std::vector<unsigned char> rasterData; // Données en format RGB
    int width;
    int height;
    bool valid;
};

#endif // GEOTIFFLOADER_H
