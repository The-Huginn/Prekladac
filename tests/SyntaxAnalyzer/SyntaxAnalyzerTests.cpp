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
    EXPECT_EQ(parseAndGenerate(input, NULL, stderr), 0);
    fclose(input);
}

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
    EXPECT_EQ(parseAndGenerate(input, NULL, stderr), 4);    // assignment of integer into number
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

// class BottomToTopTests : public ::testing::Test
// {
//     public:

//     FILE *input;
//     Node *result;
//     void SetUp() override
//     {
//         input = NULL;
//         result = NULL;
//     }

//     void TearDown() override
//     {
//         if (result != NULL)
//             Node_PostOrder(result, true);
//         fclose(input);
//         LexicalDestroy();
//     }
// };

// TEST_F(BottomToTopTests, 1_SimpleTest)
// {
//     input = fopen("../../../../tests/files/testExpression1.txt", "r");
//     EXPECT_EQ(BottomToTop(input, stderr, NULL, &result), -1);
// }

// TEST_F(BottomToTopTests, 2_SimpleTest)
// {
//     input = fopen("../../../../tests/files/testExpression2.txt", "r");
//     EXPECT_EQ(BottomToTop(input, stderr, NULL, &result), -1);
// }

// TEST_F(BottomToTopTests, 3_SimpleTest)
// {
//     input = fopen("../../../../tests/files/testExpression3.txt", "r");
//     EXPECT_EQ(BottomToTop(input, stderr, NULL, &result), -1);
// }

// TEST_F(BottomToTopTests, 4_SimpleTest)
// {
//     input = fopen("../../../../tests/files/testExpression4.txt", "r");
//     EXPECT_EQ(BottomToTop(input, stderr, NULL, &result), -1);
// }

// TEST_F(BottomToTopTests, 5_SimpleTest)
// {
//     input = fopen("../../../../tests/files/testExpression5.txt", "r");
//     EXPECT_EQ(BottomToTop(input, stderr, NULL, &result), -1);
// }

// TEST_F(BottomToTopTests, 6_SimpleTest)
// {
//     input = fopen("../../../../tests/files/testExpression6.txt", "r");
//     EXPECT_EQ(BottomToTop(input, stderr, NULL, &result), -1);
// }

// TEST_F(BottomToTopTests, 7_SimpleTest)
// {
//     input = fopen("../../../../tests/files/testExpression7.txt", "r");
//     EXPECT_EQ(BottomToTop(input, stderr, NULL, &result), -1);
// }

// TEST_F(BottomToTopTests, 8_SimpleTest)
// {
//     input = fopen("../../../../tests/files/testExpression8.txt", "r");
//     EXPECT_EQ(BottomToTop(input, stderr, NULL, &result), -1);
// }

// TEST_F(BottomToTopTests, 1_SimpleWrongTest)
// {
//     input = fopen("../../../../tests/files/testWrongExpression1.txt", "r");
//     EXPECT_EQ(BottomToTop(input, stderr, NULL, &result), 2);
// }

// // should parse as 2 separate expressions when expecting only one, Precedence table does not know this exactly
// TEST_F(BottomToTopTests, 2_SimpleWrongTest)
// {
//     input = fopen("../../../../tests/files/testWrongExpression2.txt", "r");
//     EXPECT_EQ(BottomToTop(input, stderr, NULL, &result), -1);
//     EXPECT_EQ(BottomToTop(input, stderr, NULL, &result), -1);
// }

// TEST_F(BottomToTopTests, 3_SimpleWrongTest)
// {
//     input = fopen("../../../../tests/files/testWrongExpression3.txt", "r");
//     EXPECT_EQ(BottomToTop(input, stderr, NULL, &result), 2);
// }

// TEST_F(BottomToTopTests, 4_SimpleWrongTest)
// {
//     input = fopen("../../../../tests/files/testWrongExpression4.txt", "r");
//     EXPECT_EQ(BottomToTop(input, stderr, NULL, &result), 2);
// }

// TEST_F(BottomToTopTests, 5_SimpleWrongTest)
// {
//     input = fopen("../../../../tests/files/testWrongExpression5.txt", "r");
//     EXPECT_EQ(BottomToTop(input, stderr, NULL, &result), 2);
// }

// TEST_F(BottomToTopTests, 6_SimpleWrongTest)
// {
//     input = fopen("../../../../tests/files/testWrongExpression6.txt", "r");
//     EXPECT_EQ(BottomToTop(input, stderr, NULL, &result), 2);
// }

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

