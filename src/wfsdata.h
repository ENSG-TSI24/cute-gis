#ifndef WFSDATA_H
#define WFSDATA_H

#include <gdal_priv.h>
#include <ogrsf_frmts.h>

class WFSData
{
private:
    GDALDataset* m_dataset; // Use GDALDataset instead of OGRDataSource
public:
    WFSData();
    void open(const char* link);
    void getData(const char* name);
    GDALDataset* GetDataset();
};

#endif // WFSDATA_H
