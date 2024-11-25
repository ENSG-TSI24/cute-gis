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
    wfsData.open("https://data.geopf.fr/wfs/ows?SERVICE=WFS&VERSION=2.0.0&REQUEST=GetCapabilities");

    // Retrieve and display the data
    std::cout << "Fetching data from WFS..." << std::endl;
    wfsData.getData("VIGICRUES-INT:troncons");
    std::cout << wfsData.GetDataset() << std::endl;

    return 0;
}
