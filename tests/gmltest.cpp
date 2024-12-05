#include "gtest/gtest.h"
#include "../src/back/citygmlparser.h"
#include <gdal/ogrsf_frmts.h>
#include <glm/glm.hpp>


TEST(gmltest, Constructor) {
    CityGMLParser parser = CityGMLParser();
    EXPECT_TRUE(parser.getXMin() == 10000000);
    EXPECT_TRUE(parser.getYMin() == 10000000);
    EXPECT_TRUE(parser.getXMax() == 0);
    EXPECT_TRUE(parser.getYMax() == 0);
}

TEST(gmltest, openfile) {
    CityGMLParser parser = CityGMLParser();
    const std::string& filePath = "../test_data/CHARLY_EAU_2015.gml";
    EXPECT_TRUE(parser.openFile(filePath));
    const std::string& wrongpath = "../test_data/CHARLY_EAU.gml";
    EXPECT_FALSE(parser.openFile(wrongpath));
}

TEST(gmltest, transformation) {
    CityGMLParser parser = CityGMLParser();
    OGRCoordinateTransformation* trans = parser.createLambertTransformation();

    OGRSpatialReference* sourceSR = new OGRSpatialReference();
    sourceSR->importFromEPSG(4326); // WGS84 (Lat/Lon)

    OGRSpatialReference* targetSR = new OGRSpatialReference();
    targetSR->importFromEPSG(2154); // Lambert-93 (EPSG:2154)

    OGRCoordinateTransformation* trans_ref = OGRCreateCoordinateTransformation(sourceSR, targetSR);;

    double x = 2.2945;   // Longitude
    double y = 48.8584;  // Latitude
    double z = 0.0;      // Altitude

    double x_ref = x, y_ref = y, z_ref = z;

    trans->Transform(1, &x, &y, &z);
    trans_ref->Transform(1, &x_ref, &y_ref, &z_ref);

    EXPECT_NEAR(x, x_ref, 1e-6);
    EXPECT_NEAR(y, y_ref, 1e-6);
    EXPECT_NEAR(z, z_ref, 1e-6);
}



