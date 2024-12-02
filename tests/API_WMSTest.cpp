#include "gtest/gtest.h"
#include <fstream>
#include "../src/API_WMS.h"

TEST(API_WMSTest, LoadInvalidDataset_ThrowsException) {
    const char* url = "https://google.com";
    API_WMS flux_nonvalide = API_WMS(url);
    EXPECT_THROW(flux_nonvalide.loadDataset(), std::runtime_error);

}

/////////////////////////////// 3 Subtests for getdata /////////////////////////////////////
TEST(API_WMSTest, GetDataset_ReturnsNullptr_Initially) {

    const char* url = "WMS:https://data.geopf.fr/wms-r?SERVICE=WMS&VERSION=1.3.0&REQUEST=GetCapabilities";
    API_WMS flux_valide = API_WMS(url);
    // Vérifie qu'au début, aucun dataset n'est chargé
    ASSERT_EQ(flux_valide.getDataset(), nullptr);

}

TEST(API_WMSTest, GetDataset_ReturnsNonNullptr_AfterLoadingValidDataset) {

    const char* url = "WMS:https://data.geopf.fr/wms-r?SERVICE=WMS&VERSION=1.3.0&REQUEST=GetCapabilities";
    API_WMS flux_valide = API_WMS(url);
    // Act : Charger un dataset valide
    flux_valide.loadDataset();

    // Assert : Vérifie que le dataset est chargé avec succès
    ASSERT_NE(flux_valide.getDataset(), nullptr);
    EXPECT_FALSE(flux_valide.isEmpty());
}


/////////////////////////////// Il reste les méthodes propres à WMS : l'export
TEST(API_WMSTest, DownloadTileToGeoTiff_FileGenerated) {
    //arrange
    const char* url1 = "WMS:https://data.geopf.fr/wms-r?SERVICE=WMS&VERSION=1.3.0&REQUEST=GetCapabilities";
    API_WMS flux_valide = API_WMS(url1);

    const char* layerName = "OI.OrthoimageCoverage";
    const char* outputFile = "/home/formation/minisig/cute-gis/src/tile_orthoimage.tiff"; // ** changer le lien
    int zoom = 1;
    int row = 1;
    int col = 0;

    // Act
    flux_valide.downloadTileToGeoTiff(layerName, outputFile, zoom, row, col);

    // Assert
    std::ifstream file(outputFile);
    EXPECT_TRUE(file.is_open()); // Vérifie que le fichier existe
    file.close();

    // Nettoyage après test
    std::remove(outputFile);
}
