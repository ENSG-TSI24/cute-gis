#include "gtest/gtest.h"
#include "../src/API_WMTS.h"

TEST(API_WMSTest, LoadInvalidDataset_ThrowsException) {
    const char* url_wmts = "https://nimportequoi.com";
    API_WMTS flux_nonvalide = API_WMTS(url_wmts);
    EXPECT_THROW(flux_nonvalide.loadDataset(), std::runtime_error);

}

TEST(API_WMSTest, GetDataset_ReturnsNullptr_AfterLoadingInvalidDataset) {
    const char* url_wmts = "https://nimportequoi.com";
    API_WMTS flux_nonvalide = API_WMTS(url_wmts);

    // Act : Charger un dataset invalide
    flux_nonvalide.loadDataset();

    // Assert : Vérifie qu'aucun dataset n'est chargé
    ASSERT_EQ(flux_nonvalide.getDataset(), nullptr);
    EXPECT_TRUE(flux_nonvalide.isEmpty());
}

TEST(API_WMSTest, GetDataset_ReturnsNonNullptr_AfterLoadingValidDataset) {

    const char* url_wmts = "WMTS:https://data.geopf.fr/wmts?SERVICE=WMTS&VERSION=1.0.0&REQUEST=GetCapabilities";
    API_WMTS flux_valide = API_WMTS(url_wmts);
    // Act : Charger un dataset valide
    flux_valide.loadDataset();

    // Assert : Vérifie que le dataset est chargé avec succès
    ASSERT_NE(flux_valide.getDataset(), nullptr);
    EXPECT_FALSE(flux_valide.isEmpty());
}





