#include "gtest/gtest.h"
#include "../src/API_WMS.h"

class API_WMSTest : public ::testing::Test {
protected:
    // Common instance for all tests 
    API_WMS flux_valide;    
    API_WMS flux_nonvalide;

    // Arrange before each test 
    void SetUp() override {
        const char* url1 = "WMS:https://data.geopf.fr/wms-r?SERVICE=WMS&VERSION=1.3.0&REQUEST=GetCapabilities";
        flux_valide = API_WMS(url1);

        const char* url2 = "https://google.com";
        flux_nonvalide = API_WMS(url2);
    }

    // Nettoyage après chaque test
    void TearDown() override {

        // free ressources
        delete flux_valide; 
        delete flux_nonvalide; 
    }
};

// test for loading function 
TEST_F(API_WMSTest, LoadInvalidDataset_ThrowsException) {
    EXPECT_THROW(flux_nonvalide.loadDataset(), std::runtime_error);
}

/////////////////////////////// 3 Subtests for getdata /////////////////////////////////////
TEST_F(API_WMSTest, GetDataset_ReturnsNullptr_Initially) {
    // Vérifie qu'au début, aucun dataset n'est chargé
    ASSERT_EQ(flux_valide.getDataset(), nullptr);
    ASSERT_EQ(flux_nonvalide.getDataset(), nullptr);
}
TEST_F(API_WMSTest, GetDataset_ReturnsNonNullptr_AfterLoadingValidDataset) {
    // Act : Charger un dataset valide
    flux_valide.loadDataset();

    // Assert : Vérifie que le dataset est chargé avec succès
    ASSERT_NE(flux_valide.getDataset(), nullptr);
    EXPECT_FALSE(provider.isEmpty());   
}

TEST_F(API_WMSTest, GetDataset_ReturnsNullptr_AfterLoadingInvalidDataset) {
    // Act : Charger un dataset invalide
    flux_nonvalide.loadDataset();

    // Assert : Vérifie qu'aucun dataset n'est chargé
    ASSERT_EQ(flux_nonvalide.getDataset(), nullptr);
    EXPECT_TRUE(provider.isEmpty());
}

/////////////////////////////// 3 Subtests for metadata /////// ** chercher exemples //////////
TEST_F(API_WMSTest, DisplayMetadata_WithValidDataset_ShowsMetadata) {
    // Arrange
    flux_valide.loadDataset();

    // Act & Assert
    testing::internal::CaptureStdout();
    flux_valide.displayMetadata();
    std::string output = testing::internal::GetCapturedStdout();

    // Vérifier que des métadonnées sont affichées
    ASSERT_FALSE(output.empty()); 
    ASSERT_NE(output.find("WMS MetaData"), std::string::npos); 
}

/*TEST_F(API_WMSTest, DisplayMetadata_WithoutMetadata_ShowsNoMetadataMessage) {
    // Arrange
    const char* url_no_metadata = "WMS:some-url-with-no-metadata"; // look for a stream without metadata
    API_WMS flux_no_metadata(url_no_metadata);
    flux_no_metadata.loadDataset();

    // Act & Assert
    testing::internal::CaptureStdout();
    flux_no_metadata.displayMetadata();
    std::string output = testing::internal::GetCapturedStdout();

    // Vérifiez que le message "no metadata" est affiché
    ASSERT_NE(output.find("no metadata"), std::string::npos);
}*/

TEST_F(API_WMSTest, DisplayMetadata_WithInvalidDataset_DoesNothing) {
    // Act & Assert
    testing::internal::CaptureStdout();
    flux_nonvalide.displayMetadata();
    std::string output = testing::internal::GetCapturedStdout();

    // Vérifiez qu'aucune métadonnée n'est affichée
    ASSERT_TRUE(output.empty());
}

/////////////////////////////// Il reste les méthodes propres à WMS : l'export 

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}