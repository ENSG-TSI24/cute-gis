//#include "mainwindow.h"

//#include <QApplication>
// a basculer en fonction de flux
#include "src/API_WMS.h"


int main(int argc, char *argv[])
{
    /*QApplication a(argc, argv);
    MainWindow w;
    w.show();*/
    /*
    const char* wmtsUrl = "https://data.geopf.fr/wmts?SERVICE=WMTS&VERSION=1.0.0&REQUEST=GetCapabilities";

    const char* layerName = "ORTHOIMAGERY.ORTHOPHOTOS";

    int zoomLevel = 6;

    int centerRow = 22;

    int centerCol = 32;

    int gridSize = 3;


    // Créer l'instance de API_WMTS

    API_WMTS wmts(wmtsUrl);


    // Charger le dataset et les métadonnées

    wmts.loadDataset();

    wmts.displayMetadata();

    // tests pour le flux WMS
//    QApplication a(argc, argv);
//    MainWindow w;
    w.show(); */


    const char* wmsUrl = "WMS:https://data.geopf.fr/wms-r?SERVICE=WMS&VERSION=1.3.0&REQUEST=GetCapabilities";

    const char* layerName = "OI.OrthoimageCoverage";

    API_WMS wms(wmsUrl);
    wms.loadDataset();
    wms.isEmpty();
    wms.displayMetadata();
    const char* outputFile = "/home/formation/minisig/cute-gis/src/tile_orthoimage.tiff";

    wms.downloadTileToGeoTiff(layerName,outputFile, 1,1,0);
    //return a.exec();
    return 0;

}
