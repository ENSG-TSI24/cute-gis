
#include <iostream>
#include <ogrsf_frmts.h>

#include "geojsonvisitor.h"
#include "../wfsdata.h"

GeojsonVisitor::GeojsonVisitor(){
    this->points = nullptr;
    this->linestrings = nullptr;
    this->polygons = nullptr;
}
GeojsonVisitor::~GeojsonVisitor(){}

GeoJsonVisitor::visit(GDALDataset geoJsonDataset){
    this->setPoints(geoJsonDataset);
    this->setLineStrings(geoJsonDataset);
    this->setPolygons(geoJsonDataset);
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
