#ifndef CITYGMLFILE_H
#define CITYGMLFILE_H

#include "vectordata.h"

class CityGMLFIle : public VectorData
{
public:
    CityGMLFIle();
private :
    GDALDriver GetDriver();
    GDALDataset GetDATA();
    void closeDataSet();

};

#endif // CITYGMLFILE_H
