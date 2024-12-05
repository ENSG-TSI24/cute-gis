#include "../src/back/datamanagment.h"
#include "../src/back/vectordata.h"

#include <gtest/gtest.h>
#include <gdal.h>
#include <ogrsf_frmts.h>

#include <fstream>
#include <iostream>


class DataManagmentTest : public ::testing::Test {
    protected:
        DataManagment data;
        VectorData vectordata;
        const char* inputFile = "../test_data/BASSIN_VERSANT.geojson";
};


TEST_F(DataManagmentTest, DefaultConstructor) {
    data = DataManagment();
    EXPECT_TRUE(data.GetDATA() == nullptr);
    EXPECT_TRUE(data.GetPath() == nullptr);
    GDALClose(data.GetDATA());
}

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
    VectorData vectordata = VectorData();
    EXPECT_TRUE(vectordata.GetPath() == nullptr);
}

TEST_F(DataManagmentTest, ConstructorVectorData){
    std::ifstream file(inputFile);
    if (!file.is_open()){
        throw std::runtime_error("Error: Could not open file");
    }
    file.close();
    vectordata = VectorData(inputFile);
    std::cout << inputFile << std::endl;
    std::cout << vectordata.GetDATA() << std::endl;
    auto points = vectordata.GetPoints();
    EXPECT_TRUE(points.size() !=0);
}


TEST_F(DataManagmentTest, VectorDatagetpoints) {
    vectordata = VectorData(inputFile);
    auto points = vectordata.GetPoints();
    EXPECT_TRUE(points.size() !=0);
    for (int i = 0; i<2; i++) {
    std::cout << "Point " << i << ": (" << points[i].first << ", " << points[i].second << ")" << std::endl;
    }
}

TEST_F(DataManagmentTest, VectorGetLineStrings) {
    vectordata = VectorData(inputFile);
    auto linestring = vectordata.GetLineStrings();
    EXPECT_TRUE(linestring.size() !=0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}