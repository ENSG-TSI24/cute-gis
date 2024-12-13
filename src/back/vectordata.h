#ifndef VECTORDATA_H
#define VECTORDATA_H

#include "datamanagment.h"

class VectorData : public DataManagment
{
public:
    VectorData();

    /**
    * @brief create a vectordata class assciocated to a file
    * @param const char* : path, the path to the file
    * @return VectorData
    */
    VectorData(const char* path);
    ~VectorData();

    /**
    * @brief Get all features of type polygon in the file asssociated to the VectorData instance
    * @return std::vector<std::vector<std::vector<std::tuple<float, float, float>>>>
    */
    std::vector<std::vector<std::vector<std::tuple<float, float, float>>>>GetPolygons();
    /**
    * @brief Get all features of type linestring in the file asssociated to the VectorData instance
    * @return std::vector<std::vector<std::tuple<float, float, float>>>
    */
    std::vector<std::vector<std::pair<float, float>>> GetLineStrings();
    /**
    * @brief Get all features of type point in the file asssociated to the VectorData instance
    * @return std::vector<std::tuple<float, float, float>>
    */
    std::vector<std::pair<float, float>> GetPoints();
    /**
    * @brief Same as GetPolygons(), but the z coordinate of each tuple is 0
    * @return std::vector<std::vector<std::vector<std::tuple<float, float, float>>>>
    */
    std::vector<std::vector<std::vector<std::pair<float, float>>>> Get2DPolygons();
    /**
    * @brief Get all features attributes names in the file asssociated to the VectorData instance
    * @return std::vector<std::string>
    */
    std::vector<std::string> GetAttributName();
    /**
    * @brief Get all features attributes in the file asssociated to the VectorData instance
    * @return std::vector<std::vector<std::string>>
    */
    std::vector<std::vector<std::string>> GetAllAttributData();
    /**
    * @brief Get all features attributes for one field in the file asssociated to the VectorData instance
    * @param const char *fieldName : name of the field
    * @return std::vector<std::vector<std::string>>
    */
    std::vector<std::string> GetAttributeDataByHeader(const char *fieldName);
    /**
    * @brief Get attributes for a feature in the file asssociated to the VectorData instance
    * @param int id : id of the feature
    * @return std::vector<std::vector<std::string>>
    */
    std::vector<std::vector<std::string>> GetAttributeDataById(int id);
protected:
};

#endif // VECTORDATA_H
