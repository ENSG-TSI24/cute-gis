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
        const char* inputFile =  "/home/formation/Documents/ProjetTSI/SAINT_DIDIER_AU_MONT_D_OR_BATI_2015_repaired.gml";
};

/*
TEST(CityGMLParserTest, ConstructorVectorDataNull){
    CityGMLParser parser = CityGMLParser();
    EXPECT_FALSE(parser.open(""));
}*/

TEST_F(CityGMLParserTest, ConstructorVectorDataNull){
    data = CityGMLParser();

    EXPECT_TRUE(data.GetDataset()== nullptr);
    EXPECT_TRUE(data.GetPath() == nullptr);
}

/*TEST_F(CityGMLParserTest, ConstructorVectorData){
    data = CityGMLParser(inputFile);

    EXPECT_TRUE(data.GetDataset()== nullptr);
    std::cout<<"cc:"<<data.GetDataset()<<"\n"<<std::endl;
    EXPECT_TRUE(data.GetPath() == inputFile);
    EXPECT_TRUE(data.getFeatures().size() == 0);


}
*/
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


