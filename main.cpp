#include "src/front/mainwindow.h"
#include <QApplication>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include "src/back/API_WFS.h"

using json = nlohmann::json;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // Créer et afficher la fenêtre principale
    MainWindow w;
    w.show();
    const char* wfsUrl = "WFS:https://data.geopf.fr/wfs/ows?SERVICE=WFS&VERSION=2.0.0&REQUEST=GetCapabilities";

    const char* layerName = "ADMINEXPRESS-COG.LATEST:chflieu_commune";

    API_WFS wfs(wfsUrl);
    wfs.loadDataset();

    std::cout << wfs.displayMetadata() << "";
    char** layers = wfs.displayMetadata(); // Get the list of layers

        for (int i = 0; layers[i] != nullptr; ++i) { // Iterate until null terminator
            std::cout << "Layer " << i + 1 << ": " << layers[i] << std::endl;
        }
    return app.exec();
}
