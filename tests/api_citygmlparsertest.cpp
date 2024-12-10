#include "../src/back/citygmlparser.h"
#include "gtest/gtest.h"
#include <fstream>

#include <gtest/gtest.h>
#include <gdal.h>
#include <ogrsf_frmts.h>

#include <fstream>
#include <iostream>
#include <vector>


class DataManagmentTest : public ::testing::Test {
    protected:
        GDALDataset* data;
        std::vector<Feature> features;
        const char* inputFile =  "../test_data/BASSIN_VERSANT.geojson";
};

/*
TEST_F(DataManagmentTest, DefaultConstructor) {
    data = DataManagment();
    EXPECT_TRUE(data.GetDATA() == nullptr);
    EXPECT_TRUE(data.GetPath() == nullptr);
    GDALClose(data.GetDATA());
}
*/
// TEST_F(DataManagmentTest, ConstructorWithParameters) {
//     const char* inputFile = "../test_data/BASSIN_VERSANT.geojson";
//     DataManagment data(inputFile);
//     GDALDataset *dataset = (GDALDataset *) GDALOpenEx(inputFile, GDAL_OF_VECTOR, nullptr, nullptr, nullptr);

//     EXPECT_TRUE(data.GetDATA()->GetLayerCount  ()  == dataset->GetLayerCount  () );
// //    EXPECT_TRUE(data.GetDATA()->GetGCPCount ()  == dataset->GetGCPCount () );
//     EXPECT_TRUE(data.GetPath() ==  inputFile);
//     EXPECT_TRUE(data.GetDriver() == dataset->GetDriver());
//     GDALClose(dataset);
// //    GDALClose(data.GetDATA());
// }

TEST_F(DataManagmentTest, ConstructorVectorDataNull){
}

TEST_F(DataManagmentTest, ConstructorVectorData){

}


TEST_F(DataManagmentTest, VectorDatagetpoints) {

}

TEST_F(DataManagmentTest, VectorGetLineStrings) {

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


