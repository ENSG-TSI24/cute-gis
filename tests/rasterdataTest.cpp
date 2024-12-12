#include "../src/back/rasterdata.h"
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <utility>

class RasterDataTest : public ::testing::Test {
protected:
    RasterData raster;
    const char* testFile = "/home/formation/Documents/mini_SIG/cute-gis/output_WMS/London.tif";

    void SetUp() override {
        raster = RasterData(testFile);
    }
};

TEST_F(RasterDataTest, Constructor) {
    //EXPECT_TRUE(raster.GetImage() != nullptr) << "Failed to load image from file.";
    EXPECT_EQ(raster.GetWidth(), 6001) << "Width mismatch.";
    EXPECT_EQ(raster.GetHeight(), 5001) << "Height mismatch.";
}

TEST_F(RasterDataTest, GeoCoordinatesCorners) {
    int width = raster.GetWidth();
    int height = raster.GetHeight();

    auto topLeft = raster.GetGeoCoordinates(0, 0);
    auto topRight = raster.GetGeoCoordinates(width - 1, 0);
    auto bottomLeft = raster.GetGeoCoordinates(0, height - 1);
    auto bottomRight = raster.GetGeoCoordinates(width - 1, height - 1);

}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
