#ifndef WFSDATA_H
#define WFSDATA_H

#include <gdal_priv.h>
#include <ogrsf_frmts.h>

class WFSData
{
public:
    GDALDataset* m_dataset;
    WFSData();
    void open();
    void getData();
};

#endif // WFSDATA_H
