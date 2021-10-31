/**
 * @file LexicalAnalyzer.c
 * @brief This file implements interface for communication with lexical analyzer
 * @author Rastislav Budinsky
 */
#include <string.h>
#include <stdlib.h>
#include "LexicalAnalyzer.h"
#include "LexicalFSM.h"

char *keywords[] = {"and", "boolean", "do", "else", "elseif", "end", "false", "function", "global", "if", "integer", "local", "nil", "not", "number", "or", "require", "return", "string", "then", "true", "while"};

#define KEYWORDS_SUM 22

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
    Token *token = (Token*) malloc(sizeof(Token));

    if (token == NULL)  // TO DO error msg
        return NULL;

    LexicalOutput *lexeme = getLexeme(input);

    if (getFinalState(lexeme) == ERROR_STATE)
    {
        token->type = ERROR;
    }
    else if (getFinalState(lexeme) == F_ID)
    {
        if (IsKeyWord(lexeme))
        {
            // TO DO
        }
        else
        {
            // TO DO
        }
    }
    else
    {
        // TO DO
    }

    freeLexeme(lexeme);

    return token;
}