#include "layer2d.h"

Layer2d::Layer2d(Geojsonloader data)
{
    points = data.getPoints();
    linestrings = data.getLinestrings();
    polygons = data.getPolygons();
}


Layer2d::~Layer2d(){

}
