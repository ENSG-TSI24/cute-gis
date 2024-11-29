
#ifndef WMTSDATA_H
#define WMTSDATA_H

#include <gdal_priv.h>
#include <string>
#include <vector>

class wmtsdata {
public:
    wmtsdata();
    ~wmtsdata() { close(); }

    // Ouvre un flux WMTS
    void open(const char* link);

    // Liste les couches disponibles dans le flux
    void getLayers();

    // Télécharge une tuile spécifique
    void downloadTile(const char* layerName, const char* outputFile, int zoom, int row, int col);

    // Charge une grille de tuiles autour d'une position donnée
    std::vector<std::string> loadTileGrid(const char* layerName, int zoom, int centerRow, int centerCol, int gridSize);

    // Ferme le flux
    void close();

    // Retourne le dataset actuel
    GDALDataset* GetDataset();

private:
    GDALDataset* m_dataset;
};

#endif // WMTSDATA_H