#include "gtest/gtest.h"
#include <fstream>
#include "../src/back/API_WFS.h"

TEST(API_WFSTest, LoadInvalidDataset_ThrowsException) {
    const char* url = "https://google.com";
    API_WFS flux_nonvalide = API_WFS(url);
    EXPECT_THROW(flux_nonvalide.loadDataset(), std::runtime_error);

}

/*TEST(API_WFSTest, Empty_Constructor_noURL ) {
    API_WFS flux_nonvalide = API_WFS();
    EXPECT_THROW(flux_nonvalide.loadDataset(), std::runtime_error);

}*/

TEST(API_WFSTest, GetDataset_ReturnsNullptr_Initially) {

    const char* url = "WFS:https://data.geopf.fr/wfs/ows?SERVICE=WFS&VERSION=2.0.0&REQUEST=GetCapabilities";
    API_WFS flux_valide = API_WFS(url);
    // Verify that no dataset is loaded initially
    ASSERT_EQ(flux_valide.getDataset(), nullptr);

}

TEST(API_WFSTest, GetDataset_ReturnsNonNullptr_AfterLoadingValidDataset) {

    const char* url = "WFS:https://data.geopf.fr/wfs/ows?SERVICE=WFS&VERSION=2.0.0&REQUEST=GetCapabilities";
    API_WFS flux_valide = API_WFS(url);
    // Act : load a valid dataset 
    flux_valide.loadDataset();

    // Assert : Verify the dataset is loaded successfully
    ASSERT_NE(flux_valide.getDataset(), nullptr);
    EXPECT_FALSE(flux_valide.isEmpty());
}


/////////////////////////////// Export Geojson
TEST(API_WFSTest, DownloadTileToGeoTiff_FileGenerated) {
    //arrange
    const char* url = "WFS:https://data.geopf.fr/wfs/ows?SERVICE=WFS&VERSION=2.0.0&REQUEST=GetCapabilities";
    API_WFS flux_valide = API_WFS(url);
    flux_valide.loadDataset();
    // Act
    const char* layername = "LIMITES_ADMINISTRATIVES_EXPRESS.LATEST:arrondissement";
    flux_valide.ExportToGeoJSON(layername);
    const char* outputFile = "../data/geojson/LIMITES_ADMINISTRATIVES_EXPRESS.LATEST:arrondissement.geojson"; // ** changer le lien
    flux_valide.getOutput();

    EXPECT_EQ(*outputFile, *flux_valide.getOutput());
    // Assert
    std::ifstream file(outputFile);
    EXPECT_TRUE(file.is_open()); // Verifies exported file exists
    file.close();
    std::remove(outputFile);
}
