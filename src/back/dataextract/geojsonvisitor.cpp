
#include <iostream>
#include <ogrsf_frmts.h>

#include "geojsonvisitor.h"
#include "../wfsdata.h"

GeoJsonVisitor::GeoJsonVisitor(){
    this->points =  std::vector<std::pair<float, float>> {};
    this->linestrings =  std::vector<std::vector<std::pair<float, float>>> {};
    this->polygons = std::vector<std::vector<std::vector<std::pair<float, float>>>>{};
}
GeoJsonVisitor::~GeoJsonVisitor(){
    if (this->points.size() != 0) {
        delete this->points;
    }
    if (this->linestrings.size() != 0) {
        delete this->linestrings;
    }
    if (this->polygons.size() != 0) {
        delete this->polygons;
    }
}

GeoJsonVisitor::visitGeojson(GDALDataset* dataset){
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
            this.points.emplace_back(point->getX(), point->getY());
            break;
        }
        case wkbLineString: {
            OGRLineString* lineString = geometry->toLineString();
            std::vector<std::pair<float, float>> line;
            for (int i = 0; i < lineString->getNumPoints(); ++i) {
                line.emplace_back(lineString->getX(i), lineString->getY(i));
            }
            this.linestrings.push_back(line);
            break;
        }
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
            this.polygons.push_back(polyRings);
            break;
        }
        default:
            // Géométries non supportées
            break;
        }

        // Libérer la mémoire de l'entité
        OGRFeature::DestroyFeature(feature);
    }
}

void GeojsonVisitor::setPoints(GDALDataset geoJsonDataset){
    
}
void GeojsonVisitor::setLineStrings(GDALDataset geoJsonDataset){
   
    
}
void GeojsonVisitor::setPolygons(GDALDataset geoJsonDataset){
    
}


std::vector<std::pair<float, float>> GeojsonVisitor::getPoints(){
    return this.points
}

std::vector<std::vector<std::pair<float, float>>> GeojsonVisitor::getLineStrings(){
    return this.linestrings
}

std::vector<std::vector<std::vector<std::pair<float, float>>> GeojsonVisitor::getPolygons(){
    return this.polygons
}
