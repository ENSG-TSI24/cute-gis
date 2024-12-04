#ifndef ABSTRACTDATAVISITOR_H
#define ABSTRACTDATAVISITOR_H

#include <gdal/gdal.h>
#include <ogrsf_frmts.h>

class AbstractDataVisitor {
public:
    virtual ~AbstractDataVisitor() = default;
    virtual GDALDataset* visitGeojson(GDALDataset* geoJsonDataset);
    virtual GDALDataset* visitSHP(GDALDataset* shpDataset);
    virtual GDALDataset* visitSHP(GDALDataset* shpDataset, GDALDataset* shp_intersectedDataset);
    virtual GDALDataset* visitGeojson(GDALDataset* geoJsonDataset, GDALDataset* geoJson_intersectedDataset);
};

#endif