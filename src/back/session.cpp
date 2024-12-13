#include "session.h"
#include <iostream>
#include <fstream>
#include <filesystem>

Session::Session() {
    m_json = std::make_shared<nlohmann::json>();
    std::ifstream infile("../data/session.json");
    if (infile.is_open() && infile.peek() != std::ifstream::traits_type::eof()) {
        infile >> *m_json;
    }
    infile.close();
}

Session::~Session() {
}

void Session::updateFile() {
    std::filesystem::create_directories("../data");
    std::ofstream file("../data/session.json");
    if (file.is_open()) {
        file << m_json->dump(4);
        file.close();
    } else {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier session.json pour écriture" << std::endl;
    }
}

void Session::addToJson(const char* path, const char* type) {
    if (m_json) {
        (*m_json)[path] = {
            {"type", type},
            {"properties", {
                {"name", path},
                {"visible", true}
            }}
        };
        updateFile();
    }
}

std::vector<std::pair<std::string, std::string>> Session::getLayers() {
    std::vector<std::pair<std::string, std::string>> layers;
    if (m_json) {
        for (auto& [key, layer] : m_json->items()) {
            if (layer.contains("properties") && layer["properties"].contains("name") && layer.contains("type")) {
                layers.emplace_back(layer["properties"]["name"], layer["type"]);
            }
        }
    }
    return layers;
}

void Session::removeFromJson(const char* path) {
    if (m_json) {
        for (auto it = m_json->begin(); it != m_json->end(); ) {
            if (it.value().contains("properties") && it.value()["properties"].contains("name") && it.value()["properties"]["name"].get<std::string>() == path) {
                it = m_json->erase(it);
            } else {
                ++it;
            }
        }
        updateFile();
    }
}