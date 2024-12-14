#include "../src/back/datamanagment.h"
#include "../src/back/vectordata.h"

#include <gtest/gtest.h>
#include <gdal.h>
#include <ogrsf_frmts.h>

#include <iostream>


class DataManagmentTest : public ::testing::Test {
    protected:
        DataManagment data;
        VectorData vectordata;
        const char* inputPoint = "../data/test_data/point.geojson";
        const char* inputLine = "../data/test_data/linestring.geojson";
        const char* inputMultiLine = "../data/test_data/multilinestring.geojson";
        const char* inputPolygon = "../data/test_data/polygon.geojson";
        const char* inputMultiPolygons = "../data/test_data/multipolygon.geojson";
        const char* inputFileDepartements =  "../data/test_data/departements.geojson";
        const char* inputFileLyon = "../data/geojson/polygon_nord.geojson";
};

TEST_F(DataManagmentTest, ConstructorVectorDataNull){
    VectorData vectordata = VectorData();
    EXPECT_TRUE(vectordata.GetPath() == nullptr);
}

TEST_F(DataManagmentTest, ConstructorVectorData){
    vectordata = VectorData(inputFileDepartements);
    EXPECT_TRUE(vectordata.GetPath() == inputFileDepartements);
}


TEST_F(DataManagmentTest, VectorDatagetpoints) {
    vectordata = VectorData(inputPoint);
    auto points = vectordata.GetPoints();
    EXPECT_TRUE(points.size() ==1);
}

TEST_F(DataManagmentTest, VectorDatagetpointsCoordinates) {
    vectordata = VectorData(inputPoint);
    auto points = vectordata.GetPoints();
    EXPECT_NEAR(points[0].first, -105.01621, 1e-5);
}


TEST_F(DataManagmentTest, VectorGetLine) {
    vectordata = VectorData(inputLine);
    auto linestring = vectordata.GetLineStrings();
    EXPECT_TRUE(linestring.size() !=0);
}

TEST_F(DataManagmentTest, VectorGetLineString) {
    vectordata = VectorData(inputMultiLine);
    auto linestrings = vectordata.GetLineStrings();
    EXPECT_EQ(linestrings.size(), 4);

}

TEST_F(DataManagmentTest, VectorGetPolygons) {
    vectordata = VectorData(inputPolygon);
    auto polygons = vectordata.GetPolygons();
    
    ASSERT_EQ(polygons.size(), 1);

    auto coordinates = polygons[0];
    size_t pointCount = 0;
    for (const auto& ring : coordinates) {
        pointCount += ring.size();
    }
    EXPECT_EQ(pointCount, 5);
}


TEST_F(DataManagmentTest, VectorGetMultiPolygons) {
    vectordata = VectorData(inputMultiPolygons);
    auto polygons = vectordata.GetPolygons();
    
    ASSERT_EQ(polygons.size(), 2);

}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
