#include "../src/back/rasterdata.h"
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <utility>

class RasterDataTest : public ::testing::Test {
protected:
    RasterData raster;
    const char* testFile = "../data/output_WMS/tile_orthoimage.tiff";

    void SetUp() override {
        raster = RasterData(testFile);
    }
};


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
