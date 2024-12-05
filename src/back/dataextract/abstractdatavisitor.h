#ifndef ABSTRACTDATAVISITOR_H
#define ABSTRACTDATAVISITOR_H

#include <gdal/gdal.h>
#include <ogrsf_frmts.h>

class AbstractDataVisitor {
public:
    virtual ~AbstractDataVisitor(){};
    virtual void visitGeojson(GDALDataset* geoJsonDataset);
    //virtual GDALDataset* visitSHP(GDALDataset* shpDataset);
    //virtual GDALDataset* visitSHP(GDALDataset* shpDataset, GDALDataset* shp_intersectedDataset);
    virtual GDALDataset* visitGeojson(GDALDataset* geoJsonDataset, GDALDataset* geoJson_intersectedDataset) = 0;
};

#endif
