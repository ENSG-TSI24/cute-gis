#ifndef INTERSECTVISITOR_H
#define INTERSECTVISITOR_H

#include "abstractdatavisitor.h"


class IntersectVisitor : AbstractDataVisitor {
    public:
    IntersectVisitor();
    ~IntersectVisitor();
    //GDALDataset* visitSHP(GDALDataset* shpDataset, GDALDataset* shp_intersectedDataset);
    //GDALDataset* visitGeojson(GDALDataset* geoJsonDataset, GDALDataset* geoJson_intersectedDataset);
};


#endif