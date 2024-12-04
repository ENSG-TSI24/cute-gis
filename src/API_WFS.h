#ifndef API_WFS_H
#define API_WFS_H

#include <gdal_priv.h>
#include <string>
#include <vector>
#include <ogrsf_frmts.h>
#include "DataProvider.h"

class API_WFS: public DataProvider {
public:
    API_WFS(const char* link) ;
    //~API_WMS() ;

    // Ouvre un flux WFS
    void loadDataset();
    void open(const char* link);
    void getData(const char* name);
    std::string ExportToGeoJSON(int n, const std::string& outputFilePath);
    OGRLayer* GetLayer(const char* name = "");



protected:
    const char* url;
};

#endif // API_WFS_H

