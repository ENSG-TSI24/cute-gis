#ifndef VECTORDATA_H
#define VECTORDATA_H

#include "abstractdata.h"

class VectorData : public AbstractData
{
public:
    VectorData();
    VectorData(const char* path);
    ~VectorData();
    std::vector<std::vector<std::vector<std::pair<float, float>>>> GetPolygons();
    std::vector<std::vector<std::pair<float, float>>> GetLineStrings();
    std::vector<std::pair<float, float>> GetPoints();
};

#endif // VECTORDATA_H
