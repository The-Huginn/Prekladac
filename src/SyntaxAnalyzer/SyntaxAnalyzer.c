/**
 * @file SyntaxAnalyzer.c
 * @brief This file implements interface of SyntaxAnalyzer.h, contains custom functions
 * @author Rastislav Budinsky
 */
#include "SyntaxAnalyzer.h"
#include "Terminal.h"
#include "NonTerminal.h"
#include "LLTable.h"
#include "BottomToTop.h"

#include "../Utils/stack.h"
#include "../Utils/list.h"

#include "../LexicalAnalyzer/LexicalAnalyzer.h"


#include <stdlib.h>

// #define DEBUG_SYNTAX

/**
 * @brief Top to Bottom syntax method implemented as predictive parsing
 * @param input source code stream
 * @param output assembly output stream
 * @param error_output stream for everything else
 * @param clear whether this function was called only to clear stack
 * @return -1 upon work in progress
 *          1-9 & 99 upon an error
 *          0 upon successful finish
 */
int TopToBottom(FILE *input, FILE *output, FILE *error_output, bool clear)
{
    static Stack *topToBottomStack = NULL;
    static Token *token;

    if (clear)
    {
        Stack_Destroy(topToBottomStack);
        topToBottomStack = NULL;

        // maybe Token_Destroy in the future
        if (token != NULL)
            free(token);
        token = NULL;

        return 0;
    }

    // token and stack are NULLs
    if (topToBottomStack == NULL)
    {
        topToBottomStack = Stack_Init((void (*)(void*)) Symbol_Destroy);
        if (topToBottomStack == NULL)
            return -1;

        Symbol *a = Symbol_Init(true, $);
        Stack_Push(topToBottomStack, a);

        // NT_PROGRAM is starting NonTerminal
        a = Symbol_Init(false, NT_PROGRAM);
        Stack_Push(topToBottomStack, a);

        token = getToken(input);
    }

    Symbol *top = (Symbol *) Stack_Top(topToBottomStack);

    #ifdef DEBUG_SYNTAX
        fprintf(error_output, "token [%d], stack top [%d, %d]\n", Token_getType(token), Symbol_IsTerminal(top), Symbol_GetValue(top));
    #endif
    
    if (Token_getType(token) == ERROR)
    {
        free(token);
        token = NULL;
        return 1;
    }

    if (Symbol_IsTerminal(top) && (Terminal)Symbol_GetValue(top) == $)
    {
        if (Token_getType(token) == $)
        {
            free(token);
            token = NULL;
            return 0;
        }
        else
        {
            free(token);
            token = NULL;
            return 2;
        }
    }
    else if (Symbol_IsTerminal(top))
    {
        if (Token_getType(token) == (Terminal) Symbol_GetValue(top))
        {
            Stack_Pop(topToBottomStack);
            free(token);
            token = getToken(input);
        }
        else
        {
            free(token);
            token = NULL;
            return 2;
        }
    }
    else
    {
        if (Symbol_GetValue(top) == NT_EXPRESSION)
        {
            returnToken(token);
            Stack_Pop(topToBottomStack);
            
            Node* expression = NULL;
            int ret = BottomToTop(input, output, error_output, &expression);
            // TODO with expression
            if (expression != NULL)
                Node_PostOrder(expression, true);

            token = getToken(input);
            return ret;
        }

        // -1 in index to skip ERROR enum
        int index = LLTable[Symbol_GetValue(top)][Token_getType(token) - 1];
        
        if (index == -1)
        {
            free(token);
            token = NULL;
            return 2;
        }
        
        Stack_Pop(topToBottomStack);
        for (int i = Rule_GetSize(&(rules[index])) - 1; i >= 0; i--)
        {
            Symbol *next = Symbol_Init(Symbol_IsTerminal(Rule_GetSymbol(&(rules[index]), i)), Symbol_GetValue(Rule_GetSymbol(&(rules[index]), i)));

            if (Symbol_IsTerminal(next) && Symbol_GetValue(next) == EPSILON)
            {
                Symbol_Destroy(next);
                break;
            }

            Stack_Push(topToBottomStack, next);
        }

        #ifdef DEBUG_SYNTAX
            fprintf(error_output, "Apply new rule: %d\n", index);

            if (Symbol_IsTerminal(Rule_GetSymbol(&(rules[index]), 0)))
                fprintf(error_output, "<%d> -> %d", Symbol_GetValue(&(rules[index].left_side)), Symbol_GetValue(Rule_GetSymbol(&(rules[index]), 0)));
            else
                fprintf(error_output, "<%d> -> <%d>", Symbol_GetValue(&(rules[index].left_side)), Symbol_GetValue(Rule_GetSymbol(&(rules[index]), 0)));
            for (int i = 1 ; i < Rule_GetSize(&(rules[index])); i++)
            {
                if (Symbol_IsTerminal(Rule_GetSymbol(&(rules[index]), i)))
                    fprintf(error_output, ", %d", Symbol_GetValue(Rule_GetSymbol(&(rules[index]), i)));
                else
                    fprintf(error_output, ", <%d>", Symbol_GetValue(Rule_GetSymbol(&(rules[index]), i)));
            }
            fprintf(error_output, "\n");
        #endif
    }

    return -1;
}

int parseAndGenerate(FILE *input, FILE *output, FILE *error_output)
{
    int return_value = 0;
    while (true)
    {
        int a = TopToBottom(input, output, error_output, false);

        // success
        if (a == 0)
            break;

        // error
        if (a > 0)
        {
            // only the first error is propagated as output value
            if (return_value == 0)
                return_value = a;
            break;
        }
    }

    TopToBottom(NULL, NULL, NULL, true);
    LexicalDestroy();

    // might move to main later
    fprintf(error_output, "Exit code: %d\n", return_value);

    return return_value;
}