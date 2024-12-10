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
    ~WFSData();
    void open(const char* link);
    void getData(const char* name);
    GDALDataset* GetDataset();
    std::string ExportToGeoJSON(int n, const std::string& outputFilePath);
    OGRLayer* GetLayer(const char* name = "");
};

#endif // WFSDATA_H