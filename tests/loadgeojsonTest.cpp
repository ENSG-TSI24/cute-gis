#include <gtest/gtest.h>
#include <fstream>
#include <iostream>

#include <gdal_priv.h>
#include <ogrsf_frmts.h>

class LoadGeojsonTest : public ::testing::Test {
    protected:
        wfsdata geojson;
};


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}