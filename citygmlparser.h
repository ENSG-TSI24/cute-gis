#ifndef CITYGMLPARSER_H
#define CITYGMLPARSER_H

#include <string>
#include <vector>
#include <gdal/ogrsf_frmts.h>

// Struct to represent a single feature's geometry and metadata
struct Feature {
    std::string id;                      // Unique feature ID
    std::vector<float> vertices;         // Vertex positions (x, y, z)
    std::vector<unsigned int> faces;     // Face indices for rendering
    std::vector<std::pair<std::string, std::string>> attributes; // Metadata as key-value pairs
};

class CityGMLParser {
public:
    CityGMLParser();                              // Constructor
    ~CityGMLParser();                             // Destructor

    bool openFile(const std::string& filePath);   // Open and load a CityGML file
    const std::vector<Feature>& getFeatures() const; // Retrieve all parsed features

private:
    GDALDataset* dataset;                         // GDAL dataset for the CityGML file
    std::vector<Feature> features;                // List of parsed features

    void parseFeatures();                         // Parse features from the dataset
    void extractGeometry(OGRGeometry* geometry, std::vector<float>& vertices,
                         std::vector<unsigned int>& faces, unsigned int& vertexOffset); // Extract geometry
};

#endif // CITYGMLPARSER_H
