// IFJ Project 2021
/**
 * @file LexicalAnalyzer.c
 * @brief This file implements interface for communication with lexical analyzer
 * @note keywords and terminal enum should be ordered
 * magic constants: K_AND should be the first keyword terminal
 *                  F_ASS should be the first final state
 * @author Rastislav Budinsky (xbudin05)
 */
#include <string.h>
#include <stdlib.h>
#include "LexicalAnalyzer.h"
#include "LexicalFSM.h"
#include "../Utils/stack.h"

char *keywords[] = {"and", "boolean", "break", "do", "else", "elseif", "end", "false", "for", "function", "global", "if", "integer", "local", "nil", "not", "number", "or", "repeat", "require", "return", "string", "then", "true", "until", "while"};

#define KEYWORDS_SUM 26

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
        Token *token = (Token*)Stack_Top(stack);
        Stack_Pop(stack);
        return token;
    }

    Token *token = (Token*) malloc(sizeof(Token));

    if (token == NULL)  // TO DO error msg
        return NULL;

    LexicalOutput *lexeme = getLexeme(input);
    if (lexeme == NULL)
    {
        Token_Destroy(token);
        return NULL;
    }

    if (getFinalState(lexeme) == ERROR_STATE)
        token->type = ERROR;
    else if (getFinalState(lexeme) == EOF_STATE)
        token->type = $;
    else
    {
        int keyword = IsKeyWord(lexeme);
        if (keyword != -1)
        {
            token->type = (Terminal) (K_AND + keyword);
        }
        else
        {
            // needs to be redone, one enum has to be renamed
            token->type = (Terminal)(T_ASS + getFinalState(lexeme) - F_ASS);
        }
    }

    token->attribute = strdup(getString(lexeme));
    fprintf(stderr, "%s\n", Token_getData(token));
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
        Token *token = (Token*)Stack_Top(stack);
        // token free function
        Token_Destroy(token);
        Stack_Pop(stack);
    }
    Stack_Destroy(stack);    
    stack = NULL;
}