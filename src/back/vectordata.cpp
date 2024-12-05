#include "vectordata.h"
#include <ogrsf_frmts.h>

VectorData::VectorData() : filePath(nullptr) {}

VectorData::VectorData(const char* path) : DataManagment(path) {}


VectorData::~VectorData() {
    if (data) {
        GDALClose(data);
    }
}

std::vector<std::pair<float, float>> VectorData::GetPoints(){
    std::vector<std::pair<float, float>> points;
    GDALDataset* dataset = GetDATA();
    if (!dataset) {
        throw std::runtime_error("Invalid GDALDataset pointer provided.");
    }

    // Accéder à la première couche
    OGRLayer* layer = dataset->GetLayer(0);
    if (!layer) {
        throw std::runtime_error("Failed to get layer from GDALDataset.");
    }

    // Parcourir les entités de la couche
    OGRFeature* feature;
    while ((feature = layer->GetNextFeature()) != nullptr) {
        OGRGeometry* geometry = feature->GetGeometryRef();
        if (!geometry) {
            OGRFeature::DestroyFeature(feature);
            continue;
        }

        // Traitement des géométries selon leur type
        switch (geometry->getGeometryType()) {
        case wkbPoint: {
            OGRPoint* point = geometry->toPoint();
            points.emplace_back(point->getX(), point->getY());
            break;
        }
        default:
            // Géométries non supportées
            break;
        }

        // Libérer la mémoire de l'entité
        OGRFeature::DestroyFeature(feature);
    }
    return points;
}

std::vector<std::vector<std::pair<float, float>>>  VectorData::GetLineStrings(){
    std::vector<std::vector<std::pair<float, float>>> linestrings;
    GDALDataset* dataset = GetDATA();
    if (!dataset) {
        throw std::runtime_error("Invalid GDALDataset pointer provided.");
    }

    // Accéder à la première couche
    OGRLayer* layer = dataset->GetLayer(0);
    if (!layer) {
        throw std::runtime_error("Failed to get layer from GDALDataset.");
    }

    // Parcourir les entités de la couche
    OGRFeature* feature;
    while ((feature = layer->GetNextFeature()) != nullptr) {
        OGRGeometry* geometry = feature->GetGeometryRef();
        if (!geometry) {
            OGRFeature::DestroyFeature(feature);
            continue;
        }

        // Traitement des géométries selon leur type
        switch (geometry->getGeometryType()) {
        case wkbLineString: {
            OGRLineString* lineString = geometry->toLineString();
            std::vector<std::pair<float, float>> line;
            for (int i = 0; i < lineString->getNumPoints(); ++i) {
                line.emplace_back(lineString->getX(i), lineString->getY(i));
            }
            linestrings.push_back(line);
            break;
        }
        default:
            // Géométries non supportées
            break;
        }

        // Libérer la mémoire de l'entité
        OGRFeature::DestroyFeature(feature);
    }
    return linestrings;
}

std::vector<std::vector<std::vector<std::pair<float, float>>>> VectorData::GetPolygons(){
    std::vector<std::vector<std::vector<std::pair<float, float>>>> polygons;
    GDALDataset* dataset = GetDATA();
    if (!dataset) {
        throw std::runtime_error("Invalid GDALDataset pointer provided.");
    }

    // Accéder à la première couche
    OGRLayer* layer = dataset->GetLayer(0);
    if (!layer) {
        throw std::runtime_error("Failed to get layer from GDALDataset.");
    }

    // Parcourir les entités de la couche
    OGRFeature* feature;
    while ((feature = layer->GetNextFeature()) != nullptr) {
        OGRGeometry* geometry = feature->GetGeometryRef();
        if (!geometry) {
            OGRFeature::DestroyFeature(feature);
            continue;
        }

        // Traitement des géométries selon leur type
        switch (geometry->getGeometryType()) {
        case wkbPolygon: {
            OGRPolygon* polygon = geometry->toPolygon();
            std::vector<std::vector<std::pair<float, float>>> polyRings;

            for (int i = 0; i < polygon->getNumInteriorRings() + 1; ++i) {
                const OGRLinearRing* ring = (i == 0) ? polygon->getExteriorRing() : polygon->getInteriorRing(i - 1);
                if (ring) {
                    std::vector<std::pair<float, float>> ringPoints;
                    for (int j = 0; j < ring->getNumPoints(); ++j) {
                        ringPoints.emplace_back(ring->getX(j), ring->getY(j));
                    }
                    polyRings.push_back(ringPoints);
                }
            }
            polygons.push_back(polyRings);
            break;
        }
        default:
            // Géométries non supportées
            break;
        }

        // Libérer la mémoire de l'entité
        OGRFeature::DestroyFeature(feature);
    }
    return polygons;
}