// LexicalAnalyzerTests.cpp      

extern "C" 
{
    #include "../../src/SyntaxAnalyzer/SyntaxAnalyzer.h"
}

#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

class SyntaxTests : public ::testing::Test
{
    public:

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(SyntaxTests, 1_SimpleTest)
{
    EXPECT_TRUE(true);
    FILE* input = fopen("../../../../tests/files/getLexemeTests.txt", "r");
    parseAndGenerate(input, NULL, stderr);
    fclose(input);
}

TEST_F(SyntaxTests, 2_SimpleTest)
{
    EXPECT_FALSE(false);
    FILE* input = fopen("../../../../tests/files/syntaxTest1.txt", "r");
    parseAndGenerate(input, NULL, stderr);
    fclose(input);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    /*if (argc == 1)
    {
        ::testing::GTEST_FLAG(filter) = "";
    }
    if (argc == 2)
    {
        std::string tests = "Test1.1*";
        tests += ":Test1.2*";
        ::testing::GTEST_FLAG(filter) = tests;
    }*/
    return RUN_ALL_TESTS();
}

