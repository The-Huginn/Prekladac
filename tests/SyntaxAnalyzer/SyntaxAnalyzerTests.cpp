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
    FILE* input = fopen("../../../../tests/files/testSource1.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, NULL, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 2_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource2.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, NULL, stderr), 2);
    fclose(input);
}

TEST_F(SyntaxTests, 3_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource3.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, NULL, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 4_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource4.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, NULL, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 5_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource5.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, NULL, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 6_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource6.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, NULL, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 7_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource7.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, NULL, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 8_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource8.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, NULL, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 9_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource9.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, NULL, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 1_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong1.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, NULL, stderr), 1);
    fclose(input);
}

TEST_F(SyntaxTests, 2_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong3.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, NULL, stderr), 2);
    fclose(input);
}

// unable to decide correctness of this test, whether it is syntax error or semantic erro
// but it's an error nonetheless
TEST_F(SyntaxTests, 3_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong5.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, NULL, stderr), 2);
    fclose(input);
}

TEST_F(SyntaxTests, 4_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong6.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, NULL, stderr), 1);
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

