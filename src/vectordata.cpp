#include "vectordata.h"

VectorData::VectorData()
    : AbstractData()
{
    this->GeoJsonDriver = GetGDALDriverManager()->GetDriverByName("GeoJSON");
}
