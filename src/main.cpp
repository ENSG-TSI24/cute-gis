#include <iostream>
#include "abstractdata.h"
#include "geojsonfile.h"
#include "vectordata.h"
#include "gdal_priv.h"

int main() {
    GDALAllRegister();
    const char* inputFile = "../test_data/BASSIN_VERSANT.geojson";
    GeoJsonFile data(inputFile);
    GDALDataset *dataset = (GDALDataset *) GDALOpenEx(inputFile, GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
    std::cout<<data.GetCRS()<<std::endl;
    std::cout<<data.GetCRS()<<std::endl;
    GDALClose(dataset);
//    GDALClose(data.GetDATA());
    return 0;
}
