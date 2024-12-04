//#include "mainwindow.h"

//#include <QApplication>
// a basculer en fonction de flux
#include "src/API_WFS.h"
#include <filesystem>


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


    const char* wmsUrl = "WFS:https://data.geopf.fr/wfs/ows?SERVICE=WFS&VERSION=2.0.0&REQUEST=GetCapabilities";

    const char* layerName = "ADMINEXPRESS-COG.LATEST:chflieu_commune";

    API_WFS wmts(wmsUrl);
    wmts.loadDataset();
    wmts.isEmpty();
    //wmts.displayMetadata();
    wmts.open(wmsUrl);
    //wmts.getData(layerName);
    wmts.ExportToGeoJSON(123,"../cute-gis/test.geojson");
    //return a.exec();
    return 0;

}
