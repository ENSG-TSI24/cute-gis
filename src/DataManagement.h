#include <gdal.h>
#include <gdal_priv.h>
class DataManagement {
protected :
    virtual ~DataManagement() = default;
    virtual GDALDriver GetDriver() const=0;
    virtual GDALDataset GetDATA() const=0;
    virtual void closeDataSet() const=0;
    // virtual ? GetAttribute(std::string atribute_name) const=0;
};
