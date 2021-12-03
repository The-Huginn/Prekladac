/**
 * @file main.c
 */
#include "SyntaxAnalyzer/SyntaxAnalyzer.h"
int main(int argc, char **argv)
{
    return parseAndGenerate(stdin, stdout, stderr);
}