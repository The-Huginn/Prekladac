// LexicalAnalyzerTests.cpp      

extern "C" 
{
    #include "../../src/LexicalAnalyzer/LexicalAnalyzer.h"
    #include "../../src/LexicalAnalyzer/LexicalFSM.h"
}

#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>


void TestLexeme(FILE* file, std::string req)
{
    LexicalOutput* lexeme = getLexeme(file);
    ASSERT_EQ(getString(lexeme), req);
    freeLexeme(lexeme);
}


TEST(getLexemeTests, 1_introduction) {
    FILE* input = fopen("../../../../tests/files/getLexemeTests.txt", "r");
    
    std::vector<std::string> lexemes = { "require", "\"ifj21\"", "function", "main", "(", ")", "local", "a", ":", "integer", "local", "vysl", ":", "integer", "=", "0", "write", "(", "\"Zadejte cislo pro vypocet faktorialu\\n\"", ")", "a", "=", "readi", "(", ")", "if", "a", "==", "nil", "then", "write", "(", "\"a je nil\\n\"", ")", "return", "else", "end", "if", "a", "<", "0", "then", "write", "(", "\"Faktorial nelze spocitat\\n\"", ")", "else", "vysl", "=", "1", "while", "a", ">", "0", "do", "vysl", "=", "vysl", "*", "a", "a", "=", "a", "-", "1", "end", "write", "(", "\"Vysledek je: \"", ",", "vysl", ",", "\"\\n\"", ")", "end", "end", "main", "(", ")" };

    for (std::string lexeme : lexemes)
        TestLexeme(input, lexeme);

    fclose(input);
}

TEST(Test1, 2_advanced) {
    //ASSERT_EQ(Vypis(), 0);
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

