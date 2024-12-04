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
    //~API_WFS() ;

    // Ouvre un flux WFS
    void loadDataset();

    void getData(const char* name);

    OGRLayer* GetLayer(const char* name = "");
    std::string ExportToGeoJSON(int n, const std::string& outputFilePath);



protected:
    const char* url;
};

#endif // API_WFS_H

