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

    return app.exec();
}
