#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include <jsoncpp/json/json.h>
#include "../src/loadgeojson.h"

class LoadGeojsonTest : public ::testing::Test {
    protected:
        LoadGeojson loader;
};
TEST_F(LoadGeojsonTest, LoadFromFileSuccess) {
    loader  = LoadGeojson();
    std::string filePath = "../data/lyon.geojson";
    ASSERT_TRUE(loader.loadFromFile(filePath));
}

TEST_F(LoadGeojsonTest, LoadFromFileFailure) {
    loader = LoadGeojson();
    std::string filePath = "../data/none.geojson";
    ASSERT_FALSE(loader.loadFromFile(filePath));
}

TEST_F(LoadGeojsonTest, toStringSuccess){
    loader = LoadGeojson();
    std::string filePath = "../data/lyon.geojson";
    loader.loadFromFile(filePath);
    std::string geojsonData = loader.getGeojsonData().toStyledString();
    std::string firstLine = geojsonData.substr(0, geojsonData.find('\n'));
    ASSERT_EQ(firstLine, "{");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}