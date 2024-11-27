#ifndef CITYGMLFILE_H
#define CITYGMLFILE_H

#include "vectordata.h"

class CityGMLFIle : public VectorData
{
public:
    CityGMLFIle();
private :
    GDALDriver GetDriver() override;
    GDALDataset GetDATA() override;
    void closeDataSet() override;
};

#endif // CITYGMLFILE_H
