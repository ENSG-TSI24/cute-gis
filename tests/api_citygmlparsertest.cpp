#include "../src/back/citygmlparser.h"
#include "../src/back/datamanagment.h"
#include "gtest/gtest.h"
#include <fstream>

#include <gtest/gtest.h>
#include <gdal.h>
#include <ogrsf_frmts.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <iostream>


class CityGMLParserTest : public :: testing::Test {
    protected:
        CityGMLParser data;
        std::vector<Feature> features;
        const char* inputFile =  "../data/gml/SAINT_DIDIER_AU_MONT_D_OR_PYLONE_2015.gml";
};



TEST_F(CityGMLParserTest, ConstructorVectorDataNull){
    data = CityGMLParser();

    EXPECT_TRUE(data.GetDataset()== nullptr);
    EXPECT_TRUE(data.GetPath() == nullptr);
}


TEST_F(CityGMLParserTest, ParseFeatures) {
   data = CityGMLParser(inputFile);

   data.parseFeatures();
   EXPECT_TRUE(data.GetDataset()== nullptr);
   std::vector<Feature> features = data.getFeatures();
   EXPECT_FALSE(data.getFeatures().size() == 0);
   data.exportToObj(1.0f, "outputdelavie.obj");


}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


