/**
 * @file SyntaxAnalyzer.c
 * @brief This file implements interface of SyntaxAnalyzer.h, contains custom functions
 * @author Rastislav Budinsky
 */
#include "SyntaxAnalyzer.h"
#include "Terminal.h"
#include "NonTerminal.h"
#include "LLTable.h"
#include "../Utils/stack.h"
#include "../LexicalAnalyzer/LexicalAnalyzer.h"

/**
 * @brief Top to Bottom syntax method implemented as predictive parsing
 * @param input source code stream
 * @param output assembly output stream
 * @param error_output stream for everything else
 * @return -1 upon single or multiple errors
 *          0 upon work in progress
 *          1 upon successful finish
 */
int TopToBottom(FILE *input, FILE *output, FILE *error_output)
{
    static Stack *topToBottomStack = NULL;
    if (topToBottomStack == NULL)
    {
        topToBottomStack = Stack_Init(Symbol_Destroy);
        if (topToBottomStack == NULL)
            return;

        Symbol *a = Symbol_Init(true, EOF_STATE);
        Stack_Push(topToBottomStack, a);
        // replace 10 with startin NON TERMINAL
        // a = Symbol_Init(false, 10);
        // Stack_Push(topToBottomStack, a);
    }

    static Token *token;
    Symbol *top = Stack_Top(topToBottomStack);

    if (Symbol_IsTerminal(top) && (Terminal)Symbol_getValue(top) == EOF_STATE)
    {
        if (getType(token) == $)
            return 1;
        else
            return 0; 
    }
    else if (Symbol_IsTerminal(top))
    {
        if (getType(token) == (Terminal) Symbol_getValue(top))
        {
            Stack_Pop(topToBottomStack);
            token = getToken(input);
        }
        else
        {
            return -1;
        }
    }
    else
    {
        // TODO
    }
}

void parseAndGenerate(FILE *input, FILE *output, FILE *error_output)
{
    

}