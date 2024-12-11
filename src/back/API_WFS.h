#ifndef API_WFS_H
#define API_WFS_H

#include <gdal_priv.h>
#include <string>
#include <vector>
#include <ogrsf_frmts.h>
#include "DataProvider.h"
#include "vectordata.h"
#include <cstring>


class API_WFS: public DataProvider {
public:
    API_WFS(const char* link) ;

    void loadDataset();

    void getData(const char* name);

    OGRLayer* GetLayer(const char* name = "");
    void ExportToGeoJSON(const std::string& layerName);
    const char* getOutput();

protected:
    const char* url;
    const char* output_path ;
};

#endif // API_WFS_H

