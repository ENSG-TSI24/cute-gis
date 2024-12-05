#include "gtest/gtest.h"
#include "../src/back/API_WMTS.h"



TEST(API_WMTSTest, GetDataset_ReturnsNonNullptr_AfterLoadingValidDataset) {

    const char* url_wmts = "WMTS:https://data.geopf.fr/wmts?SERVICE=WMTS&VERSION=1.0.0&REQUEST=GetCapabilities";
    API_WMTS flux_valide = API_WMTS(url_wmts);
    // Act : Charger un dataset valide
    flux_valide.loadDataset();

    // Assert : Vérifie que le dataset est chargé avec succès
    ASSERT_NE(flux_valide.getDataset(), nullptr);
    EXPECT_FALSE(flux_valide.isEmpty());
}

TEST(API_WMTSTest, GetDataset_WMTS_ReturnsNullptr_Initially) {

    const char* url = "WMTS:https://data.geopf.fr/wmts?SERVICE=WMTS&VERSION=1.0.0&REQUEST=GetCapabilities";
    API_WMTS flux_valide = API_WMTS(url);
    // Vérifie qu'au début, aucun dataset n'est chargé
    ASSERT_EQ(flux_valide.getDataset(), nullptr);

}

TEST(API_WMTSTest, GetDataset_WMTS_ReturnsNonNullptr_AfterLoadingValidDataset) {

    const char* url = "WMTS:https://data.geopf.fr/wmts?SERVICE=WMTS&VERSION=1.0.0&REQUEST=GetCapabilities";
    API_WMTS flux_valide = API_WMTS(url);
    // Act : Charger un dataset valide
    flux_valide.loadDataset();

    // Assert : Vérifie que le dataset est chargé avec succès
    ASSERT_NE(flux_valide.getDataset(), nullptr);
    EXPECT_FALSE(flux_valide.isEmpty());
}
