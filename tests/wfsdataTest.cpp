#include <gtest/gtest.h>
#include <fstream>
#include <iostream>

#include <gdal_priv.h>
#include <ogrsf_frmts.h>

#include "../src/wfsdata.h"

class WfsDatatest : public ::testing::Test {
    protected:
        WFSData wfsdata;
};

TEST_F(WfsDatatest, OpenWFSData)
    {
    wfsdata = WFSData();
    wfsdata.open("https://qgisserver.hautsdefrance.fr/cgi-bin/qgis_mapserv.fcgi?MAP=/var/www/data/qgis/applications/politique_ville.qgz&request=GetFeature&service=WFS&typename=cvcb_points&outputformat=geojson");
    wfsdata.getData("hautdefrance.geojson");
    OGRLayer* datasetlayer = wfsdata.GetLayer();
    ASSERT_NE(datasetlayer, nullptr);
    GDALClose(datasetlayer);
}

TEST_F(WfsDatatest, IsGeoJSONValid) {
    wfsdata = WFSData();
    wfsdata.open("https://qgisserver.hautsdefrance.fr/cgi-bin/qgis_mapserv.fcgi?MAP=/var/www/data/qgis/applications/politique_ville.qgz&request=GetFeature&service=WFS&typename=cvcb_points&outputformat=geojson");
    std::string geojsonContent = wfsdata.ExportToGeoJSON(0, "../data/output.geojson");

    std::ifstream file("../data/output.geojson");
    ASSERT_TRUE(file.is_open());

    std::string line;
    std::getline(file, line); // Skip the first line
    std::getline(file, line); // Read the second line
    std::cout << line << std::endl;
    ASSERT_EQ(line, "\"type\": \"FeatureCollection\",");

    file.close();
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

