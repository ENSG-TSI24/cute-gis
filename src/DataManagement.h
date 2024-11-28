#include <gdal.h>
#include <gdal_priv.h>
#include "ogrsf_frmts.h"
#include <iostream>
class DataManagement {
protected :
    virtual ~DataManagement() = default;
    virtual GDALDriver GetDriver() const=0;
    virtual GDALDataset GetDATA() const=0;
    // virtual ? GetAttribute(std::string atribute_name) const=0;
};
