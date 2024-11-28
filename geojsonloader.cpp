#include "geojsonloader.h"


using json = nlohmann::json;

Geojsonloader::Geojsonloader(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier GeoJSON : " + filePath);
    }

    json geojson;
    try {
        file >> geojson;
    } catch (const std::exception& e) {
        throw std::runtime_error("Erreur lors du parsing du fichier GeoJSON : " + std::string(e.what()));
    }

    coordinates.clear();
    if (geojson["type"] != "FeatureCollection") {
        throw std::runtime_error("Le fichier GeoJSON n'est pas de type FeatureCollection.");
    }

    for (const auto& feature : geojson["features"]) {
        if (!feature.contains("geometry")) continue;

        auto geometry = feature["geometry"];
        if (geometry["type"] == "Point" && geometry.contains("coordinates")) {
            const auto& coord = geometry["coordinates"];
            if (coord.size() >= 2) {
                coordinates.emplace_back(coord[0], coord[1]); // (longitude, latitude)
            }
        }
    }

}


std::vector<std::pair<float, float>> Geojsonloader::getCoordinates(){
    return this->coordinates;
}



