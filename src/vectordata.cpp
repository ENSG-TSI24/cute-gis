#include "vectordata.h"

VectorData::VectorData()
    : AbstractData()
{
    this->GeoJsonDriver = GetGDALDriverManager()->GetDriverByName("GeoJSON");
}
VectorData::VectorData(char* path)
    : AbstractData(path)
{
    this->GeoJsonDriver = GetGDALDriverManager()->GetDriverByName("GeoJSON");
}
