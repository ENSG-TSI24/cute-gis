#include "abstractdatavisitor.h"

class GeoJsonVisitor : AbstractDataVisitor {
public :
    GeoJsonVisitor();
    ~GeoJsonVisitor();
    std::vector<std::pair<float, float>> getPoints();
    std::vector<std::vector<std::pair<float, float>>> getLinestrings();
    std::vector<std::vector<std::vector<std::pair<float, float>>>> getPolygons;
private :    
    std::vector<std::pair<float, float>> points;
    std::vector<std::vector<std::pair<float, float>>> linestrings;
    std::vector<std::vector<std::vector<std::pair<float, float>>>> polygons;
    void visitGeojson(GDALDataset* dataset);

    std::vector<std::vector<std::vector<std::pair<float, float>>>> GetPolygons();
    std::vector<std::vector<std::pair<float, float>>> GetLineStrings();
    std::vector<std::pair<float, float>> GetPoints();
};
