#include "mainwindow.h"

#include <QApplication>
#include "src/API_WMS.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


     const char* wmsUrl = "WMS:https://data.geopf.fr/wms-r?SERVICE=WMS&VERSION=1.3.0&REQUEST=GetCapabilities";

     const char* layerName = "OI.OrthoimageCoverage";

     API_WMS wms(wmsUrl);
     const char* outputFile = "/home/formation/Documents/mini_SIG/cute-gis/src/tile_orthoimage.tiff";

     wms.downloadTileToGeoTiff(layerName,outputFile, 1,1,0);
     return 0;
}


