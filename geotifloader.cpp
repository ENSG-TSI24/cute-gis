#include "geotifloader.h"
#include <cpl_conv.h> // Pour CPLMalloc()

GeoTiffLoader::GeoTiffLoader()
    : dataset(nullptr), width(0), height(0), valid(false) {
    GDALAllRegister(); // Enregistrer les pilotes GDAL
}

GeoTiffLoader::~GeoTiffLoader() {
    if (dataset) {
        GDALClose(dataset);
    }
}

bool GeoTiffLoader::loadGeoTiff(const std::string& filePath) {
    // Fermer le dataset précédent si nécessaire
    if (dataset) {
        GDALClose(dataset);
        dataset = nullptr;
    }

    dataset = (GDALDataset*)GDALOpen(filePath.c_str(), GA_ReadOnly);
    if (!dataset) {
        valid = false;
        return false;
    }

    width = dataset->GetRasterXSize();
    height = dataset->GetRasterYSize();

    // Vérifie si l'image a au moins 3 bandes pour RGB
    if (dataset->GetRasterCount() < 3) {
        GDALClose(dataset);
        dataset = nullptr;
        valid = false;
        return false;
    }

    // Redimensionner le vecteur pour contenir les données RGB
    rasterData.resize(width * height * 3); // 3 canaux pour RGB

    // Lire les bandes une par une
    for (int bandIndex = 1; bandIndex <= 3; ++bandIndex) {
        GDALRasterBand* band = dataset->GetRasterBand(bandIndex);
        if (!band) {
            GDALClose(dataset);
            dataset = nullptr;
            valid = false;
            return false;
        }

        // Remplir le canal correspondant dans rasterData
        band->RasterIO(GF_Read, 0, 0, width, height,
                       rasterData.data() + (bandIndex - 1), // Décalage pour le canal
                       width, height, GDT_Byte, 3, width * 3);
    }

    valid = true;
    return true;
}

bool GeoTiffLoader::isValid() const {
    return valid;
}

const std::vector<unsigned char>& GeoTiffLoader::getRasterData() const {
    return rasterData;
}

int GeoTiffLoader::getWidth() const {
    return width;
}

int GeoTiffLoader::getHeight() const {
    return height;
}
