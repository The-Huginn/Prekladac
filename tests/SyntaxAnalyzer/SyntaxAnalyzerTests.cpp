// LexicalAnalyzerTests.cpp      

extern "C" 
{
    #include "../../src/SyntaxAnalyzer/SyntaxAnalyzer.h"
    #include "../../src/SyntaxAnalyzer/BottomToTop.h"
    #include "../../src/LexicalAnalyzer/LexicalAnalyzer.h"
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

TEST_F(SyntaxTests, 0_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource0.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 1_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource1.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 2_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource2.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 3_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource3.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 4_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource4.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 5_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource5.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 6_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource6.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 7_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource7.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 8_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource8.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 9_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource9.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 10_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource10.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 11_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource11.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 12_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource12.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 13_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource13.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 0);
    fclose(input);
}


TEST_F(SyntaxTests, 14_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource14.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 15_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource15.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 16_SimpleTest)
{
    FILE* input = fopen("../../../../tests/files/testSource16.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 0);
    fclose(input);
}

TEST_F(SyntaxTests, 1_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong1.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 1);
    fclose(input);
}

TEST_F(SyntaxTests, 2_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong2.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 3);    // assignment of integer into number
    fclose(input);
}

TEST_F(SyntaxTests, 3_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong3.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 4);    // assignment of integer into number
    fclose(input);
}

TEST_F(SyntaxTests, 4_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong4.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 3);    // assignment of integer into number
    fclose(input);
}

// unable to decide correctness of this test, whether it is syntax error or semantic erro
// but it's an error nonetheless
TEST_F(SyntaxTests, 5_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong5.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 2);
    fclose(input);
}

TEST_F(SyntaxTests, 6_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong6.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 1);
    fclose(input);
}

TEST_F(SyntaxTests, 7_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong7.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 5);
    fclose(input);
}

TEST_F(SyntaxTests, 8_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong8.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 4);
    fclose(input);
}

TEST_F(SyntaxTests, 9_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong9.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 5);
    fclose(input);
}

TEST_F(SyntaxTests, 10_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong10.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 5);
    fclose(input);
}

TEST_F(SyntaxTests, 11_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong11.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 3);
    fclose(input);
}

TEST_F(SyntaxTests, 12_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong12.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 9);
    fclose(input);
}

TEST_F(SyntaxTests, 13_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong13.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 4);
    fclose(input);
}

TEST_F(SyntaxTests, 14_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong14.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 9);
    fclose(input);
}

TEST_F(SyntaxTests, 15_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong15.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 7);
    fclose(input);
}

TEST_F(SyntaxTests, 16_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong16.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 5);
    fclose(input);
}

TEST_F(SyntaxTests, 17_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong17.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 3);
    fclose(input);
}

TEST_F(SyntaxTests, 18_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong18.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 3);
    fclose(input);
}

TEST_F(SyntaxTests, 19_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong19.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 3);
    fclose(input);
}

TEST_F(SyntaxTests, 20_SimpleWrongTest)
{
    FILE* input = fopen("../../../../tests/files/testSourceWrong20.txt", "r");
    EXPECT_EQ(parseAndGenerate(input, stdout, stderr), 3);
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
    // ::testing::GTEST_FLAG(filter) = "SyntaxTests.3_SimpleWrongTest";
    return RUN_ALL_TESTS();
}

