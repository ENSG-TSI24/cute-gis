#include "gtest/gtest.h"
#include "../src/abstractdata.h"
#include "../src/vectordata.h"
#include "../src/geojsonfile.h"
#include "gdal.h"
#include "ogrsf_frmts.h"


TEST(AbstractDatatest, DefaultConstructor) {
    AbstractData data;
    EXPECT_TRUE(data.GetDATA() == nullptr);
    EXPECT_TRUE(data.GetPath() == nullptr);
    GDALClose(data.GetDATA());
}


TEST(AbstractDatatest, ConstructorWithParameters) {
    GDALAllRegister();
    const char* inputFile = "../test_data/BASSIN_VERSANT.geojson";
    AbstractData data(inputFile);
    GDALDataset *dataset = (GDALDataset *) GDALOpenEx(inputFile, GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
    EXPECT_TRUE(true);
    EXPECT_TRUE(data.GetDATA()->GetLayerCount  ()  == dataset->GetLayerCount  () );
//    EXPECT_TRUE(data.GetDATA()->GetGCPCount ()  == dataset->GetGCPCount () );
    EXPECT_TRUE(data.GetPath() == inputFile);
    EXPECT_TRUE(data.GetDriver() == dataset->GetDriver());
    GDALClose(dataset);
//    GDALClose(data.GetDATA());
}

TEST(GeoJsonFileTest, GettersGeoJsonFile) {
    GDALAllRegister();
    const char* inputFile = "../test_data/BASSIN_VERSANT.geojson";
    GeoJsonFile data(inputFile);
    GDALDataset *dataset = (GDALDataset *) GDALOpenEx(inputFile, GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
    std::cout<<data.GetCRS()<<std::endl;
//    std::cout<<data.GetMetadata()<<std::endl;
//    data.PrintGeoJsonAttributes();
//    char epsg = "2154";
//    EXPECT_TRUE(data.GetCRS() == epsg);
    GDALClose(dataset);
}
