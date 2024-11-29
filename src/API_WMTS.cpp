
#include "API_WMTS.h"
#include <iostream>
#include <gdal_priv.h>
#include <cpl_conv.h> // Pour CPLMalloc()
#include <vector>
#include <sstream>

wmtsdata::wmtsdata() : m_dataset(nullptr) {}

GDALDataset* wmtsdata::GetDataset() {
    return this->m_dataset;
}

void wmtsdata::open(const char* link) {
    // Ouvrir le dataset avec GDAL pour un flux WMTS
    m_dataset = static_cast<GDALDataset*>(
        GDALOpen(link, GA_ReadOnly));

    if (m_dataset == nullptr) {
        std::cerr << "Échec de l'ouverture du flux WMTS : " << link << std::endl;
        exit(1);
    }
}

void wmtsdata::getLayers() {
    if (m_dataset == nullptr) {
        std::cerr << "Le flux WMTS n'est pas ouvert." << std::endl;
        return;
    }

    // Récupérer les métadonnées des couches disponibles
    char** metadata = m_dataset->GetMetadata("SUBDATASETS");
    if (metadata == nullptr) {
        std::cerr << "Aucune couche WMTS disponible." << std::endl;
        return;
    }

    std::cout << "Couches disponibles dans le flux WMTS :" << std::endl;

    for (int i = 0; metadata[i] != nullptr; i += 2) {
                std::string subdataset(metadata[i]); // Convertir le char* en std::string
                size_t pos = subdataset.find("layer="); // Trouver l'emplacement de "layer="
                if (pos != std::string::npos) {
                    std::string layerName = subdataset.substr(pos + 6); // Extraire la partie après "layer="
                    std::cout << layerName << std::endl;
                }
            }
}

void wmtsdata::downloadTile(const char* layerName, const char* outputFile, int zoom, int row, int col) {
    if (m_dataset == nullptr) {
        std::cerr << "Le flux WMTS n'est pas ouvert." << std::endl;
        return;
    }

    // Construire l'URL pour accéder à une tuile spécifique avec les paramètres WMTS
    std::ostringstream tileUrlStream;
    tileUrlStream << "https://data.geopf.fr/wmts?SERVICE=WMTS&VERSION=1.0.0&REQUEST=GetTile"
                  << "&LAYER=" << layerName
                  << "&STYLE=normal"
                  << "&FORMAT=image/jpeg"
                  << "&TILEMATRIX=" << zoom
                  << "&TILEMATRIXSET=PM"  // Le tile matrix set peut être spécifique à chaque service WMTS
                  << "&TILEROW=" << row
                  << "&TILECOL=" << col;

    std::string tileUrl = tileUrlStream.str();

    // Afficher la requête envoyée au serveur
    std::cout << "Requête envoyée au serveur : " << tileUrl << std::endl;

    // Charger la tuile en tant que dataset
    GDALDataset* tileDataset = static_cast<GDALDataset*>(
        GDALOpen(tileUrl.c_str(), GA_ReadOnly));

    if (tileDataset == nullptr) {
        std::cerr << "Échec de l'ouverture de la tuile WMTS." << std::endl;
        return;
    }

    // Sauvegarder la tuile sous forme de fichier (ex : PNG ou TIFF)
    GDALDriver* driver = GetGDALDriverManager()->GetDriverByName("PNG");
    if (driver == nullptr) {
        std::cerr << "Pilote GDAL PNG introuvable." << std::endl;
        GDALClose(tileDataset);
        return;
    }

    if (driver->CreateCopy(outputFile, tileDataset, FALSE, nullptr, nullptr, nullptr) == nullptr) {
        std::cerr << "Échec de la sauvegarde de la tuile WMTS." << std::endl;
    } else {
        std::cout << "Tuile WMTS sauvegardée dans : " << outputFile << std::endl;
    }

    // Libérer la mémoire
    GDALClose(tileDataset);
}


std::vector<std::string> wmtsdata::loadTileGrid(const char* layerName, int zoom, int centerRow, int centerCol, int gridSize) {
    std::vector<std::string> tilePaths;

    if (m_dataset == nullptr) {
        std::cerr << "Le flux WMTS n'est pas ouvert." << std::endl;
        return tilePaths;
    }

    // Calculer les limites de la grille
    int startRow = centerRow - gridSize / 2;
    int startCol = centerCol - gridSize / 2;
    int endRow = centerRow + gridSize / 2;
    int endCol = centerCol + gridSize / 2;

    for (int row = startRow; row <= endRow; ++row) {
        for (int col = startCol; col <= endCol; ++col) {
            std::ostringstream outputFileStream;
            outputFileStream << "tile_" << zoom << "_" << row << "_" << col << ".png";
            std::string outputFile = outputFileStream.str();

            downloadTile(layerName, outputFile.c_str(), zoom, row, col);
            tilePaths.push_back(outputFile);
        }
    }

    return tilePaths;
}

void wmtsdata::close() {
    if (m_dataset != nullptr) {
        GDALClose(m_dataset);
        m_dataset = nullptr;
    }
}