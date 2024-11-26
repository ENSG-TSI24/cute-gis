#include "mainwindow.h"
#include <QApplication>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>

using json = nlohmann::json;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Charger le fichier GeoJSON et gérer les erreurs
    const std::string geoJsonFile = "/home/formation/Bureau/test opengl/points_lumineux_bordeaux.geojson";
    try {
        // Tester si le fichier GeoJSON est accessible
        std::ifstream file(geoJsonFile);
        if (!file.is_open()) {
            throw std::runtime_error("Impossible d'ouvrir le fichier GeoJSON : " + geoJsonFile);
        }
        file.close();
    } catch (const std::exception& ex) {
        std::cerr << "Erreur : " << ex.what() << std::endl;
        return -1; // Terminer le programme si le fichier est inaccessible
    }

    // Créer et afficher la fenêtre principale
    MainWindow w;
    w.show();

    return app.exec();
}
