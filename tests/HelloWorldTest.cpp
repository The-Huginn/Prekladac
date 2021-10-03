// HelloWorldTest.cpp                                                                                                                                                                                                       
#include "../src/HelloWorld.c"
#include <gtest/gtest.h>
#include <iostream>
#include <string>

TEST(Test1, 1_introduction) {
    ASSERT_EQ(Vypis(), 0);
}

TEST(Test1, 2_advanced) {
    ASSERT_EQ(Vypis(), 0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    if (argc == 1)
    {
        ::testing::GTEST_FLAG(filter) = "";
    }
    if (argc == 2)
    {
        std::string tests = "Test1.1*";
        tests += ":Test1.2*";
        ::testing::GTEST_FLAG(filter) = tests;
    }
    return RUN_ALL_TESTS();
}

