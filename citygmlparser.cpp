#include "citygmlparser.h"

CityGMLParser::CityGMLParser() : dataset(nullptr) {}

CityGMLParser::~CityGMLParser() {
    if (dataset) {
        GDALClose(dataset);
    }
}

bool CityGMLParser::openFile(const std::string& filePath) {
    GDALAllRegister();
    dataset = static_cast<GDALDataset*>(GDALOpenEx(filePath.c_str(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr));
    if (!dataset) {
        std::cerr << "Error: Could not open file: " << filePath << std::endl;
        return false;
    }
    return true;
}

void CityGMLParser::extractGeometry(OGRGeometry* geometry, std::vector<float>& vertices,
                                    std::vector<unsigned int>& faces, unsigned int& vertexOffset) {
    if (!geometry) return;

    OGRwkbGeometryType geomType = geometry->getGeometryType();

    if (geomType == wkbMultiSurface || geomType == wkbMultiPolygon) {
        OGRMultiSurface* multiSurface = geometry->toMultiSurface();
        if (multiSurface) {
            for (auto i = 0; i < multiSurface->getNumGeometries(); i++) {
                OGRGeometry* subGeometry = multiSurface->getGeometryRef(i);
                extractGeometry(subGeometry, vertices, faces, vertexOffset);
            }
        }
    } else if (geomType == wkbPolygon) {
        OGRPolygon* polygon = geometry->toPolygon();
        if (polygon) {
            OGRLinearRing* ring = polygon->getExteriorRing();
            if (ring) {
                unsigned int startVertex = vertices.size() / 3;
                for (int i = 0; i < ring->getNumPoints(); i++) {
                    OGRPoint point;
                    ring->getPoint(i, &point);



