#include <gdal_priv.h>
#include <cpl_conv.h>

#include <string>
#include <iostream>
#include <fstream>
/* 
test : 
https://data.geopf.fr/wms-r?"
                         "LAYERS=OI.OrthoimageCoverage&EXCEPTIONS=text/xml&"
                         "FORMAT=image/jpeg&SERVICE=WMS&"
                         "VERSION=1.3.0&REQUEST=GetMap&STYLES=&CRS=EPSG:4326&"
                         "BBOX=47.34956960,3.25167353,47.38545104,3.30486151&WIDTH=256&HEIGHT=256"
*/

int main() {
    GDALAllRegister(); // Initialisation de GDAL

    // should I add WMS: before HTTP or not ( in QGIS it doesn't work with WMS)
    const char* wms_url = "https://data.geopf.fr/wms-r?LAYERS=OI.OrthoimageCoverage&EXCEPTIONS=text/xml&FORMAT=image/jpeg&SERVICE=WMS&VERSION=1.3.0&REQUEST=GetMap&STYLES=&CRS=EPSG:4326&BBOX=47.34956960,3.25167353,47.38545104,3.30486151&WIDTH=256&HEIGHT=256";

    // Ouvrir la connexion au flux WMS ** turn it into a constructor ( ici gdal_of_all car vecteurs ou raster possible pour WMS)
    GDALDataset* dataset = static_cast<GDALDataset*>(
    GDALOpenEx(wms_url,GDAL_OF_ALL, nullptr, nullptr, nullptr));


    if (dataset == nullptr) {
        std::cerr << "Error: Impossible de se connecter au flux WMS ou format non pris en charge." << std::endl;
        return 1;
    }
    
    
    // ** test part : to check is well loaded 
    std::cout << "Bands number : " << dataset->GetRasterCount() << std::endl;
    std::cout << "Projection : " << dataset->GetProjectionRef() << std::endl;

    // Metadata ** turn it into a function and add attributes 
    char** metadata = dataset->GetMetadata(); 
    if (metadata == nullptr) {
        std::cout << "no metadata for this WMS dataset" <<std::endl;
        
    } else {
       std::cout << "Métadonnées WMS : " << std::endl;
        for (int i = 0; metadata[i] != nullptr; ++i) {
            std::cout << metadata[i] << std::endl;
        } 
    }

    // Extraire les bandes raster qui contiennent des png ** so we need to check url passed which format it has 
    for (int i = 1; i <= dataset->GetRasterCount(); ++i) {
        GDALRasterBand* band = dataset->GetRasterBand(i);    // ** Element 
        if (band != nullptr) {
            /*std::cout << "Band " << i << " : Min=" << band->GetMinimum() 
                      << ", Max=" << band->GetMaximum() << std::endl;*/

            // Save each band as an image output (exemple : GeoTIFF)
            const std::string output_file = "output_band_" + std::to_string(i) + ".png";
            GDALDriver* driver = GetGDALDriverManager()->GetDriverByName("png");
            if (driver != nullptr) {
                GDALDataset* outDataset = driver->CreateCopy(output_file.c_str(), dataset, FALSE, nullptr, nullptr, nullptr);
                if (outDataset != nullptr) {
                    std::cout << "Bande sauvegardée sous : " << output_file << std::endl;
                    GDALClose(outDataset);
                }
            }
        }
    }

    // ** turn it into a destructor
    GDALClose(dataset);

    return 0;
}
