// IFJ Project 2021
/**
 * @file SyntaxAnalyzer.h
 * @brief This file declares interface for syntax analyzer to parse through the source code and generate assembly code
 * @author Rastislav Budinsky (xbudin05)
 */
#ifndef __SYNTAX_ANALYZER_H__
#define __SYNTAX_ANALYZER_H__

#include <stdio.h>

/**
 * @brief checks source code and generates assembly code
 * @param input source code stream
 * @param output assembly output stream
 * @param error_output stream for everything else
 * @return exit code
 */
int parseAndGenerate(FILE *input, FILE *output, FILE* error_output);


#endif // !__SYNTAX_ANALYZER_H__