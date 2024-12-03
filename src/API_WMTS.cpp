
#include "API_WMTS.h"
#include <iostream>
#include <gdal_priv.h>
#include <cpl_conv.h> // Pour CPLMalloc()
#include <vector>
#include <sstream>
#include <QImage>
#include <QGraphicsPixmapItem>

 API_WMTS:: API_WMTS(const char* link) : url(link) {
     GDALAllRegister();
 }

 void API_WMTS::loadDataset() {
     // Ouvrir le dataset avec GDAL pour un flux WMS
     m_dataset = static_cast<GDALDataset*>(
         GDALOpenEx(url,GDAL_OF_ALL, nullptr, nullptr, nullptr));

     /* Display error message and exit program if dataset fails to open correctly ** to be replaced
         when the front end team finishes ( to reinsert or close window) */
     if (isEmpty()) {
         std::cout << "Error: Impossible to connect to WMS or unsupported format" << std::endl;
         exit(1); // ** to be replaced later
     }
 }

 QImage API_WMTS::getTileAsImage(const char* layerName, int zoom, int row, int col) {
     std::ostringstream tileUrlStream;
     tileUrlStream << "https://data.geopf.fr/wmts?SERVICE=WMTS&VERSION=1.0.0&REQUEST=GetTile"
                   << "&LAYER=" << layerName
                   << "&STYLE=normal"
                   << "&FORMAT=image/jpeg"  // Assurez-vous que le format est correct
                   << "&TILEMATRIX=" << zoom
                   << "&TILEMATRIXSET=PM"
                   << "&TILEROW=" << row
                   << "&TILECOL=" << col;

     std::string tileUrl = tileUrlStream.str();
     std::cout << "Requête envoyée au serveur : " << tileUrl << std::endl;

     GDALDataset* tileDataset = static_cast<GDALDataset*>(GDALOpen(tileUrl.c_str(), GA_ReadOnly));
     if (!tileDataset) {
         std::cerr << "Échec de l'ouverture de la tuile WMTS : " << tileUrl << std::endl;
         return QImage();
     }

     int width = tileDataset->GetRasterXSize();
     int height = tileDataset->GetRasterYSize();
     int bands = tileDataset->GetRasterCount();

     if (bands < 1 || bands > 4) {
         std::cerr << "Format d'image non pris en charge : " << bands << " bandes." << std::endl;
         GDALClose(tileDataset);
         return QImage();
     }

     unsigned char* buffer = new unsigned char[width * height * 4];  // Pour RGBA, il y a 4 canaux

     if (bands == 1) {
         std::cout << "1 bande" << std::endl;
         GDALRasterBand* band = tileDataset->GetRasterBand(1);
         CPLErr err = band->RasterIO(GF_Read, 0, 0, width, height, buffer, width, height, GDT_Byte, 0, 0);
         if (err != CE_None) {
             std::cerr << "Erreur lors de la lecture des données raster (grayscale)." << std::endl;
             delete[] buffer;
             GDALClose(tileDataset);

             return QImage();
         }

         // Convertir les données en image RGB
         for (int i = 0; i < width * height; ++i) {
             buffer[i * 4] = buffer[i];     // R
             buffer[i * 4 + 1] = buffer[i]; // G
             buffer[i * 4 + 2] = buffer[i]; // B
             buffer[i * 4 + 3] = 255;       // Alpha (pleinement opaque)
         }
     } else if (bands == 3) {
         std::cout << "3 bandes" << std::endl;
         GDALRasterBand* bandR = tileDataset->GetRasterBand(1);
         GDALRasterBand* bandG = tileDataset->GetRasterBand(2);
         GDALRasterBand* bandB = tileDataset->GetRasterBand(3);

         CPLErr errR = bandR->RasterIO(GF_Read, 0, 0, width, height, buffer, width, height, GDT_Byte, 3, 0);
         CPLErr errG = bandG->RasterIO(GF_Read, 0, 0, width, height, buffer + 1, width, height, GDT_Byte, 3, 0);
         CPLErr errB = bandB->RasterIO(GF_Read, 0, 0, width, height, buffer + 2, width, height, GDT_Byte, 3, 0);

         if (errR != CE_None || errG != CE_None || errB != CE_None) {
             std::cerr << "Erreur lors de la lecture des données raster (RGB)." << std::endl;
             delete[] buffer;
             GDALClose(tileDataset);

             return QImage();
         }

         // Ajouter un canal alpha par défaut (pleinement opaque)
         for (int i = 0; i < width * height; ++i) {
             buffer[i * 4 + 3] = 255; // Alpha
         }
     } else if (bands == 4) {  // Si l'image est déjà en RGBA
          std::cout << "4 bandes" << std::endl;
         GDALRasterBand* bandR = tileDataset->GetRasterBand(1);
         GDALRasterBand* bandG = tileDataset->GetRasterBand(2);
         GDALRasterBand* bandB = tileDataset->GetRasterBand(3);
         GDALRasterBand* bandA = tileDataset->GetRasterBand(4);

         CPLErr errR = bandR->RasterIO(GF_Read, 0, 0, width, height, buffer, width, height, GDT_Byte, 4, 0);
         CPLErr errG = bandG->RasterIO(GF_Read, 0, 0, width, height, buffer + 1, width, height, GDT_Byte, 4, 0);
         CPLErr errB = bandB->RasterIO(GF_Read, 0, 0, width, height, buffer + 2, width, height, GDT_Byte, 4, 0);
         CPLErr errA = bandA->RasterIO(GF_Read, 0, 0, width, height, buffer + 3, width, height, GDT_Byte, 4, 0);

         if (errR != CE_None || errG != CE_None || errB != CE_None || errA != CE_None) {
             std::cerr << "Erreur lors de la lecture des données raster (RGBA)." << std::endl;
             delete[] buffer;
             GDALClose(tileDataset);
             std::cout << "4 bandes" << std::endl;
             return QImage();
         }
     }

     QImage img(buffer, width, height, QImage::Format_RGBA8888);
     GDALClose(tileDataset);
     delete[] buffer;

     return img;
 }

 void API_WMTS::displayTiles(const char* layerName, int zoom, int centerRow, int centerCol, int gridSize, QGraphicsScene* scene) {
     if (!scene) {
         std::cerr << "Erreur : Scène graphique invalide." << std::endl;
         return;
     }

     if (m_dataset == nullptr) {
         std::cerr << "Erreur : Le flux WMTS n'est pas ouvert." << std::endl;
         return;
     }

     // Définir les dimensions supposées de chaque tuile (WMTS standard : 256x256 pixels)
     const int tileSize = 256;

     // Calculer la position de départ en pixels pour centrer la grille
     int startX = -(gridSize / 2) * tileSize;
     int startY = -(gridSize / 2) * tileSize;

     for (int row = 0; row < gridSize; ++row) {
         for (int col = 0; col < gridSize; ++col) {
             int wmtsRow = centerRow + row - (gridSize / 2);
             int wmtsCol = centerCol + col - (gridSize / 2);

             // Charger la tuile
             QImage tileImage = getTileAsImage(layerName, zoom, wmtsRow, wmtsCol);
             if (!tileImage.isNull()) {
                 // Convertir l'image en QPixmap
                 QPixmap pixmap = QPixmap::fromImage(tileImage);

                 // Ajouter l'image à la scène
                 QGraphicsPixmapItem* pixmapItem = scene->addPixmap(pixmap);

                 // Positionner l'image correctement dans la grille
                 pixmapItem->setPos(startX + col * tileSize, startY + row * tileSize);

                 std::cout << "Tuile chargée : "
                           << "row=" << wmtsRow << ", col=" << wmtsCol
                           << " à la position (" << (startX + col * tileSize) << ", " << (startY + row * tileSize) << ")"
                           << std::endl;
             } else {
                 std::cerr << "Impossible de charger la tuile : row=" << wmtsRow << ", col=" << wmtsCol << std::endl;
             }
         }
     }
 }
