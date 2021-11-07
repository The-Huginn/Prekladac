/**
 * @file LexicalAnalyzer.c
 * @brief This file implements interface for communication with lexical analyzer
 * @note keywords and terminal enum should be ordered
 * magic constants: K_AND should be the first keyword terminal
 *                  F_ASS should be the first final state
 * @author Rastislav Budinsky
 */
#include <string.h>
#include <stdlib.h>
#include "LexicalAnalyzer.h"
#include "LexicalFSM.h"
#include "../Utils/stack.h"

char *keywords[] = {"and", "boolean", "do", "else", "elseif", "end", "false", "function", "global", "if", "integer", "local", "nil", "not", "number", "or", "require", "return", "string", "then", "true", "while"};

#define KEYWORDS_SUM 22

static Stack *stack = NULL;

/**
 * @brief decides whether the lexeme is a keyword
 * @param lexeme pointer to the lexem struct we want to check
 * @return returns index of keyword in array, else -1
 */
int IsKeyWord(LexicalOutput *lexeme)
{
    for (int i = 0; i < KEYWORDS_SUM; i++)
    {
        if (strcmp(getString(lexeme), keywords[i]) == 0)
            return i;
    }

    return -1;
}

Token *getToken(FILE *input)
{
    if (stack == NULL)
    {
        stack = Stack_Init((void (*)(void*)) NULL);
        if (stack == NULL)
            return NULL;
    }

    if (!Stack_IsEmpty(stack))
    {
        Token *token = Stack_Top(stack);
        Stack_Pop(stack);
        return token;
    }

    Token *token = (Token*) malloc(sizeof(Token));

    if (token == NULL)  // TO DO error msg
        return NULL;

    LexicalOutput *lexeme = getLexeme(input);
    if (lexeme == NULL)
    {
        free(token);
        return NULL;
    }

    if (getFinalState(lexeme) == ERROR_STATE)
        token->type = ERROR;
    else if (getFinalState(lexeme) == EOF_STATE)
        token->type = END_OF_FILE;
    else
    {
        int keyword = IsKeyWord(lexeme);
        if (keyword != -1)
        {
            token->type = K_AND + keyword;
        }
        else
        {
            // needs to be redone, one enum has to be renamed
            token->type = NT_ASS + getFinalState(lexeme) - F_ASS;

            // need to send value
            if (getFinalState(lexeme) == F_ID ||
                getFinalState(lexeme) == F_INTEGER ||
                getFinalState(lexeme) == F_NUMBER ||
                getFinalState(lexeme) == F_STRING)
            {
                // token->attribute = strdup(getString(lexeme));
                // token->attribute = symtable.getElement(getString(lexeme));
            }
        }
    }

    freeLexeme(lexeme);

    return token;
}

void returnToken(Token *token)
{
    if (stack == NULL)
        return;

    Stack_Push(stack, token);
}

void LexicalDestroy()
{
    if (stack == NULL)
        return;

    while (!Stack_IsEmpty(stack))
    {
        Token *token = Stack_Top(stack);
        // token free function
        Stack_Pop(stack);
    }

    Stack_Destroy(stack);    
}