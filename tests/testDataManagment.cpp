#include "../src/back/datamanagment.h"
#include "../src/back/vectordata.h"

#include <gtest/gtest.h>
#include <gdal.h>
#include <ogrsf_frmts.h>

#include <fstream>
#include <iostream>


class DataMangmentTest : public ::testing::Test {
    protected:
        DataManagment data;
        VectorData vectordata;
};


TEST_F(DataMangmentTest, DefaultConstructor) {
    data = DataManagment();
    EXPECT_TRUE(data.GetDATA() == nullptr);
    EXPECT_TRUE(data.GetPath() == nullptr);
    GDALClose(data.GetDATA());
}


TEST(DataManagmentTest, ConstructorWithParameters) {
    const char* inputFile = "../test_data/BASSIN_VERSANT.geojson";
    DataManagment data(inputFile);
    GDALDataset *dataset = (GDALDataset *) GDALOpenEx(inputFile, GDAL_OF_VECTOR, nullptr, nullptr, nullptr);

    EXPECT_TRUE(data.GetDATA()->GetLayerCount  ()  == dataset->GetLayerCount  () );
//    EXPECT_TRUE(data.GetDATA()->GetGCPCount ()  == dataset->GetGCPCount () );
    EXPECT_TRUE(data.GetPath() ==  inputFile);
    EXPECT_TRUE(data.GetDriver() == dataset->GetDriver());
    GDALClose(dataset);
//    GDALClose(data.GetDATA());
}

TEST_F(DataMangmentTest, VectorDatagetpoints) {
    const char* inputFile = "../test_data/BASSIN_VERSANT.geojson";
    vectordata = VectorData(inputFile);
    auto points = vectordata.GetPoints();
    EXPECT_TRUE(points.size() !=0);
    for (int i = 0; i<points.size(); )
}

TEST_F(DataMangmentTest, VectorGetLineStrings) {

    const char* inputFile = "../test_data/BASSIN_VERSANT.geojson";
    vectordata = VectorData(inputFile);
    auto linestring = vectordata.GetLineStrings();
    EXPECT_TRUE(linestring.size() !=0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}