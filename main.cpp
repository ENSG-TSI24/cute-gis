#include "src/wfsdata.h"
#include <iostream>
#include <ogrsf_frmts.h>

int main()
{
    // Initialize GDAL
    GDALAllRegister();

    // Create WFSData object
    WFSData wfsData;

    // Open the WFS dataset using the new URL
    std::cout << "Opening WFS dataset..." << std::endl;
    wfsData.open("https://qgisserver.hautsdefrance.fr/cgi-bin/qgis_mapserv.fcgi?MAP=/var/www/data/qgis/applications/politique_ville.qgz&request=GetFeature&service=WFS&typename=cvcb_points&outputformat=geojson");
    // Retrieve and display the data
    std::cout << "Fetching data from WFS..." << std::endl;
    wfsData.getData("");
    std::string geoJSON = wfsData.ExportToGeoJSON(0);
        if (!geoJSON.empty())
        {
            std::cout << "GeoJSON output:" << std::endl;
            std::cout << geoJSON << std::endl;
        }

    return 0;
}
