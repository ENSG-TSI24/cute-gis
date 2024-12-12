#ifndef VECTORDATA_H
#define VECTORDATA_H

#include "datamanagment.h"

class VectorData : public DataManagment
{
public:
    VectorData();
    VectorData(const char* path);
    ~VectorData();
    std::vector<std::vector<std::vector<std::tuple<float, float, float>>>>GetPolygons();
    std::vector<std::vector<std::pair<float, float>>> GetLineStrings();
    std::vector<std::pair<float, float>> GetPoints();
    std::vector<std::vector<std::vector<std::pair<float, float>>>> Get2DPolygons();

    std::vector<std::string> GetAttributName();

    std::vector<std::vector<std::string>> GetAllAttributData();
    std::vector<std::string> GetAttributeDataByHeader(const char *fieldName);
    std::vector<std::vector<std::string>> GetAttributeDataById(int id);
protected:
};

#endif // VECTORDATA_H
