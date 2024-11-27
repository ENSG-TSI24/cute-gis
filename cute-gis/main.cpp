#include <iostream>
#include <gdal.h>
#include <ogrsf_frmts.h>

int main() {
    // Initialiser GDAL
    GDALAllRegister();

    // Chemin du fichier source (Shapefile) et fichier de sortie (GeoJSON)
    const char *inputFile = "./data/COURS_D_EAU.shp";
    const char *outputFile = "./data/COURS_D_EAU.geojson";

    // Ouvrir le fichier source
    GDALDataset *sourceDataset = (GDALDataset *) GDALOpenEx(inputFile, GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
    if (sourceDataset == nullptr) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier source.\n";
        return 1;
    }

    // Créer un fichier GeoJSON (fichier de sortie)
    GDALDriver *geojsonDriver = GetGDALDriverManager()->GetDriverByName("GeoJSON");
    if (geojsonDriver == nullptr) {
        std::cerr << "Erreur : Impossible de trouver le driver GeoJSON.\n";
        GDALClose(sourceDataset);
        return 1;
    }

    GDALDataset *geojsonDataset = geojsonDriver->CreateCopy(outputFile, sourceDataset, FALSE, nullptr, nullptr, nullptr);
    if (geojsonDataset == nullptr) {
        std::cerr << "Erreur : Échec de la conversion en GeoJSON.\n";
        GDALClose(sourceDataset);
        return 1;
    }

    // Fermer les fichiers
    GDALClose(geojsonDataset);
    GDALClose(sourceDataset);

    std::cout << "Conversion réussie : " << outputFile << "\n";
    return 0;
}
