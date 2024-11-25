#include <gdal_priv.h>
#include "cpl_conv.h"
#include <string>
#include <iostream>
#include <fstream>


/* version1 where user will input an XML file that will be captured from file browser
element that the frontend team will build */
int main(){

    GDALAllRegister(); //GDAL's initialisation

    // **change it later to be entered via QT browser element ( add saving code with the name of the user's file in the same dir) 
    const std::string path_toXML = "examplefile.xml";  

    // XML file's verification with std::fstream
    std::ifstream file(path_toXML);
    if (!file.is_open()) {
        std::cerr << "Error : Impossible to open XML file " << path_toXML << "." << std::endl; // change it later to be entered via QT label element for example
        return 1;
    }

    // GDAL Construction 
    GDALDataset* dataset = (GDALDataset*)GDALOpen(path_toXML.c_str(), GA_ReadOnly);
    if (dataset == nullptr) {
        std::cerr << "Error : Impossible to open XML file or unsupported format" << std::endl;
        return 1;
    }

    //Just to ensure my code worked ( Remove it later )
    std::cout << "Nombre de bandes : " << dataset->GetRasterCount() << std::endl;
    std::cout << "Projection : " << dataset->GetProjectionRef() << std::endl;

    // Cleaning
    GDALClose(dataset);
    return 0;
}






