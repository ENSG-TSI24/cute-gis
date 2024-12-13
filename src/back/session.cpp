#include "session.h"
#include <fstream>
#include <vector>
#include <iostream>

Session::Session() {
    m_json = std::make_shared<nlohmann::json>();
    std::ifstream file("../data/session.json");
    if (file.peek() != std::ifstream::traits_type::eof()) {
        file >> *m_json;
    }
}
Session::~Session() {
}

void Session::updateFile() {
    std::ofstream file("../data/session.json");
    file << m_json->dump(4);
    file.close();
}

void Session::addToJson(const char* path) {
    (*m_json)[path] = {
        {"type", "layer"},
        {"properties", {
            {"name", path},
            {"visible", true}
        }}
    };
    updateFile();
}

std::vector<std::string> Session::getLayers() {
    std::vector<std::string> layers;
    for (auto& [key, layer] : m_json->items()) {
        if (layer.contains("properties") && layer["properties"].contains("name")) {
            layers.push_back(layer["properties"]["name"]);
        }
    }
    return layers;
}