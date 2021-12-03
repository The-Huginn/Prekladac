// IFJ Project 2021
/**
 * @file main.c
 * @author Rastislav Budinsky (xbudin05)
 */
#include "SyntaxAnalyzer/SyntaxAnalyzer.h"
int main(int argc, char **argv)
{
    return parseAndGenerate(stdin, stdout, stderr);
}