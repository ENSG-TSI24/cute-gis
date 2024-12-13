#include "session.h"
#include <iostream>
#include <fstream>
#include <filesystem>

Session::Session() {
    m_json = std::make_shared<nlohmann::json>();
    std::ifstream infile("../data/session.json");
    if (infile.peek() != std::ifstream::traits_type::eof()) {
        infile >> *m_json;
    }
    infile.close();
}

Session::~Session() {
}



void Session::updateFile() {
    std::ofstream file("../data/session.json");
    file << m_json->dump(4);
    file.close();
}

void Session::addToJson(const char* path, const char* type) {
    (*m_json)[path] = {
        {"type", type},
        {"properties", {
            {"name", path},
            {"visible", true}
        }}
    };
    updateFile();
}

std::vector<std::pair<std::string, std::string>> Session::getLayers() {
    std::vector<std::pair<std::string, std::string>> layers;
    for (auto& [key, layer] : m_json->items()) {
        if (layer.contains("properties") && layer["properties"].contains("name") && layer.contains("type")) {
            layers.emplace_back(layer["properties"]["name"], layer["type"]);
        }
    }
    return layers;
}

void Session::removeFromJson(const char* path) {
    for (auto it = m_json->begin(); it != m_json->end(); ) {
        if (it.value().contains("properties") && it.value()["properties"].contains("name") && it.value()["properties"]["name"].get<std::string>().find(path) != std::string::npos) {
            it = m_json->erase(it);
        } else {
            ++it;
        }
    }
    updateFile();
}