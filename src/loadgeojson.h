#ifndef LOADGEOJSON_H
#define LOADGEOJSON_H

#include <string>
#include <vector>
#include <jsoncpp/json/json.h>

class LoadGeojson {
public:
    LoadGeojson();
    ~LoadGeojson();

    bool loadFromFile(const std::string& filePath);
    Json::Value getGeojsonData() const { return geojsonData; }

private:
    Json::Value geojsonData;
};

#endif // LOADGEOJSON_H