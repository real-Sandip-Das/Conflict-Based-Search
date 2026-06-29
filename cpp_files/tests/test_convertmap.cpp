#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include "../include/ConvertMap.h"
#include "../include/AStar.h"

TEST(ConvertMapTest, ValidMap) {
    std::string test_file = "test_map.map";
    std::ofstream out(test_file);
    out << "type octile\nheight 3\nwidth 3\nmap\n.T@\nO.S\nW.G\n";
    out.close();

    auto result = convert_map(test_file);
    ASSERT_TRUE(result.has_value());
    
    auto map = result.value();
    ASSERT_EQ(map.size(), 3);
    ASSERT_EQ(map[0].size(), 3);
    
    EXPECT_EQ(map[0][0], '.');
    EXPECT_EQ(map[0][1], 'T');
    EXPECT_EQ(map[0][2], '@');
    EXPECT_EQ(map[1][0], 'O');
    EXPECT_EQ(map[1][1], '.');
    EXPECT_EQ(map[1][2], 'S');
    EXPECT_EQ(map[2][0], 'W');
    EXPECT_EQ(map[2][1], '.');
    EXPECT_EQ(map[2][2], 'G');

    std::remove(test_file.c_str());
}

TEST(ConvertMapTest, InvalidMap) {
    auto result = convert_map("non_existent_map.map");
    EXPECT_FALSE(result.has_value());
}


