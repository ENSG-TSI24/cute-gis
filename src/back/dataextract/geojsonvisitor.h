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
    void visit(GDALDataset data);
    void setPoints(std::vector<std::pair<float, float>> points);
    void setLineStrings(std::vector<std::vector<std::pair<float, float>>> linestrings);
    void setPolygons(std::vector<std::vector<std::vector<std::pair<float, float>>>> polygons);
};