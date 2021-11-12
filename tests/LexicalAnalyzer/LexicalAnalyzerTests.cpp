// LexicalAnalyzerTests.cpp      

extern "C" 
{
    #include "../../src/LexicalAnalyzer/LexicalAnalyzer.h"
    #include "../../src/LexicalAnalyzer/LexicalFSM.h"
    #include "../../src/SyntaxAnalyzer/Terminal.h"
    #include "../../src/Utils/Token.h"
}

#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

class LexicalTests : public ::testing::Test
{
    public :

    void TestLexeme(std::pair<std::string, Terminal> req)
    {
        LexicalOutput* lexeme = getLexeme(input);
        ASSERT_EQ(getString(lexeme), req.first);
        freeLexeme(lexeme);
    }

    void TestToken(Terminal req)
    {
        Token *token = getToken(input);
        ASSERT_EQ(Token_getType(token), req);
        free(token);
    }

    FILE *input;

    void SetUp() override
    {

    }

    void TearDown() override
    {
        fclose(input);
        LexicalDestroy();
    }
};


TEST_F(LexicalTests, 1_ExpectCorrect) {
    input = fopen("../../../../tests/files/testSource1.txt", "r");
    
    std::vector<std::pair<std::string, Terminal>> lexemes = { {"require", K_REQUIRE}, {"\"ifj21\"", T_STRING}, {"function", K_FUNCTION}, {"main", T_ID}, {"(", T_LEFT}, {")", T_RIGHT}, {"local", K_LOCAL}, {"a", T_ID}, {":", T_DEF}, {"integer", K_INTEGER}, {"local", K_LOCAL}, {"vysl", T_ID}, {":", T_DEF}, {"integer", K_INTEGER}, {"=", T_ASS}, {"0", T_INTEGER}, {"write", T_ID}, {"(", T_LEFT}, {"\"Zadejte cislo pro vypocet faktorialu\\n\"", T_STRING}, {")", T_RIGHT}, {"a", T_ID}, {"=", T_ASS}, {"readi", T_ID}, {"(", T_LEFT}, {")", T_RIGHT}, {"if", K_IF}, {"a", T_ID}, {"==", T_EQ}, {"nil", K_NIL}, {"then", K_THEN}, {"write", T_ID}, {"(", T_LEFT}, {"\"a je nil\\n\"", T_STRING}, {")", T_RIGHT}, {"return", K_RETURN}, {"else", K_ELSE}, {"end", K_END}, {"if", K_IF}, {"a", T_ID}, {"<", T_LESS}, {"0", T_INTEGER}, {"then", K_THEN}, {"write", T_ID}, {"(", T_LEFT}, {"\"Faktorial nelze spocitat\\n\"", T_STRING}, {")", T_RIGHT}, {"else", K_ELSE}, {"vysl", T_ID}, {"=", T_ASS}, {"1", T_INTEGER}, {"while", K_WHILE}, {"a", T_ID}, {">", T_GRT}, {"0", T_INTEGER}, {"do", K_DO}, {"vysl", T_ID}, {"=", T_ASS}, {"vysl", T_ID}, {"*", T_MUL}, {"a", T_ID}, {"a", T_ID}, {"=", T_ASS}, {"a", T_ID}, {"-", T_MINUS}, {"1", T_INTEGER}, {"end", K_END}, {"write", T_ID}, {"(", T_LEFT}, {"\"Vysledek je: \"", T_STRING}, {",", T_COMMA}, {"vysl", T_ID}, {",", T_COMMA}, {"\"\\n\"", T_STRING}, {")", T_RIGHT}, {"end", K_END}, {"end", K_END}, {"main", T_ID}, {"(", T_LEFT}, {")", T_RIGHT} };

    for (auto lexeme : lexemes)
        TestLexeme(lexeme);
}

TEST_F(LexicalTests, 2_ExpectCorrect) {
    input = fopen("../../../../tests/files/testSource1.txt", "r");

    std::vector<Terminal> lexemes = { K_REQUIRE, T_STRING, K_FUNCTION, T_ID, T_LEFT, T_RIGHT, K_LOCAL, T_ID, T_DEF, K_INTEGER, K_LOCAL, T_ID, T_DEF, K_INTEGER, T_ASS, T_INTEGER, T_ID, T_LEFT, T_STRING, T_RIGHT, T_ID, T_ASS, T_ID, T_LEFT, T_RIGHT, K_IF, T_ID, T_EQ, K_NIL, K_THEN, T_ID, T_LEFT, T_STRING, T_RIGHT, K_RETURN, K_ELSE, K_END, K_IF, T_ID, T_LESS, T_INTEGER, K_THEN, T_ID, T_LEFT, T_STRING, T_RIGHT, K_ELSE, T_ID, T_ASS, T_INTEGER, K_WHILE, T_ID, T_GRT, T_INTEGER, K_DO, T_ID, T_ASS, T_ID, T_MUL, T_ID, T_ID, T_ASS, T_ID, T_MINUS, T_INTEGER, K_END, T_ID, T_LEFT, T_STRING, T_COMMA, T_ID, T_COMMA, T_STRING, T_RIGHT, K_END, K_END, T_ID, T_LEFT, T_RIGHT };

    for (auto lexeme : lexemes)
        TestToken(lexeme);
}

TEST_F(LexicalTests, 3_ExpectCorrect) {
    input = fopen("../../../../tests/files/testSource2.txt", "r");

    std::vector<Terminal> lexemes = { K_GLOBAL, T_ID, T_DEF, K_FUNCTION, T_LEFT, K_STRING, T_RIGHT, T_DEF, K_STRING, K_FUNCTION, T_ID, T_LEFT, T_ID, T_DEF, K_STRING, T_RIGHT, T_DEF, K_STRING, K_RETURN, T_ID, T_LEFT, T_ID, T_RIGHT, K_END, K_FUNCTION, T_ID, T_LEFT, T_ID, T_DEF, K_STRING, T_RIGHT, T_DEF, K_STRING, K_RETURN, T_ID, T_LEFT, T_ID, T_RIGHT, K_END };

    for (auto lexeme : lexemes)
        TestToken(lexeme);
}

TEST_F(LexicalTests, 4_ExpectCorrect) {
    input = fopen("../../../../tests/files/testSource3.txt", "r");

    std::vector<Terminal> lexemes = {K_REQUIRE, T_STRING, K_FUNCTION, T_ID, T_LEFT, T_ID, T_DEF, K_STRING, T_COMMA, T_ID, T_DEF, K_STRING, T_RIGHT, T_DEF, K_STRING, T_COMMA, K_INTEGER, K_RETURN, T_ID, T_CONCAT, T_ID, T_COMMA, T_INTEGER, K_END, K_FUNCTION, T_ID, T_LEFT, T_RIGHT, K_LOCAL, T_ID, T_DEF, K_STRING, K_LOCAL, T_ID, T_DEF, K_INTEGER, T_ID, T_COMMA, T_ID, T_ASS, T_ID, T_LEFT, T_STRING, T_COMMA, T_STRING, T_RIGHT, K_END, T_ID, T_LEFT, T_RIGHT};

    for (auto lexeme : lexemes)
        TestToken(lexeme);
}

TEST_F(LexicalTests, 5_ExpectCorrect) {
    input = fopen("../../../../tests/files/testSource5.txt", "r");

    TestLexeme({"require", K_REQUIRE});
    TestLexeme({"\"ifj21\"", T_STRING});

    fclose(input);

    input = fopen("../../../../tests/files/testSource5.txt", "r");

    TestToken(K_REQUIRE);
    TestToken(T_STRING);
}

TEST_F(LexicalTests, 6_ExpectCorrect) {
    input = fopen("../../../../tests/files/testSource6.txt", "r");

    std::vector<Terminal> lexemes = {K_FUNCTION, T_ID, T_LEFT, T_RIGHT, K_RETURN, T_INTEGER, K_END, T_ID};
}

TEST_F(LexicalTests, 1_ExpectWrong) {
    input = fopen("../../../../tests/files/testSourceWrong1.txt", "r");

    std::vector<Terminal> lexemes = {K_REQUIRE, T_STRING, K_FUNCTION, T_ID, T_LEFT, T_RIGHT, K_RETURN, ERROR};

    for (auto lexeme : lexemes)
        TestToken(lexeme);
}

// This test is not exactly parsing wrong source but rather long identifier, which should be considered correct with flaw of possible overflow
TEST_F(LexicalTests, 2_ExpectWrong) {
    input = fopen("../../../../tests/files/testSourceWrong2.txt", "r");

    std::vector<Terminal> lexemes = {T_ID, T_ID};

    for (auto lexeme : lexemes)
        TestToken(lexeme);
}

TEST_F(LexicalTests, 3_ExpectWrong) {
    input = fopen("../../../../tests/files/testSourceWrong3.txt", "r");

    std::vector<Terminal> lexemes = {K_REQUIRE, T_STRING, K_GLOBAL, T_ID, T_DEF, K_FUNCTION, T_LEFT, T_RIGHT, K_FUNCTION, T_ID, T_LEFT, T_RIGHT, K_LOCAL, T_ID, T_DEF, K_INTEGER, K_LOCAL, T_ID, T_DEF, K_NUMBER, T_ASS, T_INTEGER, T_ID, T_ID, T_ASS, T_NUMBER, T_ID, T_ASS, T_NUMBER, ERROR};

    for (auto lexeme : lexemes)
        TestToken(lexeme);
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

