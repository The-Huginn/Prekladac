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

#define FORMAT_RULE(first, ...) ((Symbol_IsTerminal(first)) ? ", %d" : ", <%d>") FORMAT_RULE(__VA__ARGS__)
#define PRINT_RULE(format, left, first, ...) fprint(stderr, format "%d" FORMAT_RULE(__VA_ARGS__))

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
    static Token *token;

    // token and stack are NULLs
    if (topToBottomStack == NULL)
    {
        topToBottomStack = Stack_Init((void (*)(void*))Symbol_Destroy);
        if (topToBottomStack == NULL)
            return -1;

        Symbol *a = Symbol_Init(true, $);
        Stack_Push(topToBottomStack, a);

        // NT_PROGRAM is starting NonTerminal
        a = Symbol_Init(false, NT_PROGRAM);
        Stack_Push(topToBottomStack, a);

        token = getToken(input);
    }

    Symbol *top = Stack_Top(topToBottomStack);
    
    if (Token_getType(token) == ERROR)
        return -1;

    if (Symbol_IsTerminal(top) && (Terminal)Symbol_GetValue(top) == $)
    {
        if (Token_getType(token) == $)
            return 1;
        else
            return 0; 
    }
    else if (Symbol_IsTerminal(top))
    {
        if (Token_getType(token) == (Terminal) Symbol_GetValue(top))
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
        int index = LLTable[Symbol_GetValue(top)][Token_getType(token)];
        
        if (index == -1)
            return -1;
        
        Stack_Pop(topToBottomStack);
        for (int i = Rule_GetSize(&(rules[index])) - 1; i >= 0; i--)
        {
            Stack_Push(topToBottomStack, Rule_GetSymbol(&(rules[index]), i));
        }

        fprintf(error_output, "%d -> %d", Symbol_GetValue(&(rules[index].left_side)), Symbol_GetValue(Rule_GetSymbol(&(rules[index]), 0)));
        for (int i = 0 ; i < Rule_GetSize(&(rules[index])); i++)
        {
            fprintf(error_output, ", %d", Symbol_GetValue(Rule_GetSymbol(&(rules[index]), i)));
        }
        fprintf(error_output, "\n");
    }

    return 0;
}

void parseAndGenerate(FILE *input, FILE *output, FILE *error_output)
{
    while (TopToBottom(input, output, stderr) != 0);
}