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
        const char* inputFile =  "/home/jbolivier/Documents/ProjetGeo/cute-gis/test_data/BASSIN_VERSANT.geojson";
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
    VectorData vectordata = VectorData();
    EXPECT_TRUE(vectordata.GetPath() == nullptr);
}

TEST_F(DataManagmentTest, ConstructorVectorData){
    vectordata = VectorData(inputFile);
    std::cout << "input file :" << inputFile << std::endl;
    auto points = vectordata.GetPoints();
    EXPECT_TRUE(points.size() ==0);
    //std::cout << "point n : " << points[0].second << std::endl;
}


TEST_F(DataManagmentTest, VectorDatagetpoints) {
    vectordata = VectorData(inputFile);
    auto points = vectordata.GetPolygons();
    EXPECT_TRUE(points.size() !=0);

    std::cout << "number of polygon : " << points.size() << std::endl;

}

TEST_F(DataManagmentTest, VectorGetLineStrings) {
    vectordata = VectorData(inputFile);
    auto linestring = vectordata.GetLineStrings();
    EXPECT_TRUE(linestring.size() ==0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
