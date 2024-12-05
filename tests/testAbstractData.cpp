#include "gtest/gtest.h"
#include "../src/back/abstractdata.h"
#include "../src/back/vectordata.h"
#include "gdal.h"
#include "ogrsf_frmts.h"


TEST(AbstractDatatest, DefaultConstructor) {
    AbstractData data;
    EXPECT_TRUE(data.GetDATA() == nullptr);
    EXPECT_TRUE(data.GetPath() == nullptr);
    GDALClose(data.GetDATA());
}


TEST(AbstractDatatest, ConstructorWithParameters) {
    const char* inputFile = "../test_data/BASSIN_VERSANT.geojson";
    AbstractData data(inputFile);
    GDALDataset *dataset = (GDALDataset *) GDALOpenEx(inputFile, GDAL_OF_VECTOR, nullptr, nullptr, nullptr);

    EXPECT_TRUE(data.GetDATA()->GetLayerCount  ()  == dataset->GetLayerCount  () );
//    EXPECT_TRUE(data.GetDATA()->GetGCPCount ()  == dataset->GetGCPCount () );
    EXPECT_TRUE(data.GetPath() ==  inputFile);
    EXPECT_TRUE(data.GetDriver() == dataset->GetDriver());
    GDALClose(dataset);
//    GDALClose(data.GetDATA());
}

TEST(AbstractDatatest, VectorDatagetpoints) {
    const char* inputFile = "../test_data/BASSIN_VERSANT.geojson";
    auto vectordata(inputFile);
    auto points = vectordata.GetPoints();
}