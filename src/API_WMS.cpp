#include "API_WMS.h"
#include <curl/curl.h>
#include <iostream>
#include <sstream>

/* user will input an XML file that will be captured from file browser element
that the frontend team will build ** to be added in the constructor */
API_WMS::API_WMS(const char* link) : url(link)
{
    GDALAllRegister();
}

// Crucial to free memory, otherwise bugs
/*API_WMS::~API_WMS() {
    // Si le dataset existe, on le ferme
    if (m_dataset != nullptr) {
        //m_dataset->FlushCache();  // Facultatif : vide le cache du dataset avant de le fermer
        GDALClose(m_dataset);     // Libère la mémoire associée au dataset
        //m_dataset = nullptr;      // Nullifie le pointeur pour éviter les références ultérieures
    }
}*/

void API_WMS::loadDataset() {
    // Ouvrir le dataset avec GDAL pour un flux WMS
    m_dataset = static_cast<GDALDataset*>(
        GDALOpenEx(url,GDAL_OF_ALL, nullptr, nullptr, nullptr));

    /* Display error message and exit program if dataset fails to open correctly ** to be replaced
        when the front end team finishes ( to reinsert or close window) */
    if (isEmpty()) {
        std::cerr << "Error: Impossible to connect to WMS or unsupported format" << std::endl;
        throw std::runtime_error("Failed to load dataset"); // Lancer une exception au lieu d'exit
    }
}


void API_WMS::downloadTileToGeoTiff(const char* layerName, const char* outputFile, int zoom, int row, int col) {
    // Construct the URL for the WMS request with the given parameters
    std::stringstream url;
    url << "https://data.geopf.fr/wms-r?"
        << "LAYERS=" << layerName
        << "&EXCEPTIONS=text/xml&"
        << "FORMAT=image/tiff&SERVICE=WMS&VERSION=1.3.0&REQUEST=GetMap&STYLES=&"
        << "CRS=EPSG:4326&"
        << "BBOX=47.34956960,3.25167353,47.38545104,3.30486151&WIDTH=256&HEIGHT=256";

    std::cout << "Request URL: " << url.str() << std::endl;

    // Use CURL to download the image
    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());

        FILE* fp = fopen(outputFile, "wb");
        if (fp) {
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
            }
            fclose(fp);
        } else {
            std::cerr << "Unable to open output file for writing." << std::endl;
        }
        curl_easy_cleanup(curl);
    }

    // Open the downloaded image with GDAL
    GDALDataset* downloadedDataset = static_cast<GDALDataset*>(GDALOpen(outputFile, GA_ReadOnly));
    if (downloadedDataset) {
        // Crée un fichier GeoTIFF temporaire
        std::string tempFile = std::string(outputFile) + ".temp.tif";

        GDALDriver* tiffDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
        if (tiffDriver) {
            GDALDataset* tiffDataset = tiffDriver->CreateCopy(tempFile.c_str(), downloadedDataset, FALSE, nullptr, nullptr, nullptr);
            if (tiffDataset) {
                std::cout << "GeoTIFF successfully created: " << tempFile << std::endl;
                GDALClose(tiffDataset);

                // Remplace l'ancien fichier
                std::rename(tempFile.c_str(), outputFile);
            } else {
                std::cerr << "Failed to create GeoTIFF." << std::endl;
            }
        }
        GDALClose(downloadedDataset);
    } else {
        std::cerr << "Failed to open the downloaded image with GDAL." << std::endl;
    }

}

void API_WMS::loadTileGridToGeoTiff(const char* layerName, int zoom, int centerRow, int centerCol, int gridSize, const char* outputPrefix) {
    for (int row = centerRow - gridSize / 2; row <= centerRow + gridSize / 2; ++row) {
        for (int col = centerCol - gridSize / 2; col <= centerCol + gridSize / 2; ++col) {
            std::stringstream outputFile;
            outputFile << outputPrefix << "_zoom" << zoom << "_row" << row << "_col" << col << ".tiff";
            downloadTileToGeoTiff(layerName, outputFile.str().c_str(), zoom, row, col);
        }
    }
}

/*API_WMS::~API_WMS() {
    // Si le dataset existe, on le ferme
    if (m_dataset != nullptr) {
        //m_dataset->FlushCache();  // Facultatif : vide le cache du dataset avant de le fermer
        GDALClose(m_dataset);     // Libère la mémoire associée au dataset
        //m_dataset = nullptr;      // Nullifie le pointeur pour éviter les références ultérieures
    }
}*/



