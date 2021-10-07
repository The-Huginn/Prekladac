// LexicalAnalyzerTests.cpp      

#ifdef __cplusplus
    extern "C" {
#endif
#include "../../src/LexicalAnalyzer/LexicalAnalyzer.h"
#include "../../src/LexicalAnalyzer/LexicalFSM.h"

#ifdef __cplusplus
    }
#endif

#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <stdio.h>

TEST(getLexemeTests, 1_introduction) {
    FILE* input = fopen("../../../tests/files/getLexemeTests.txt", "r");
    
    LexicalOutput * lexem = getLexeme(input);
    std::string lexem_string = getString(lexem);
    std::cout << lexem_string << std::endl;

    //ASSERT_EQ(lexem_string, )

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

