/**
 * @file Lexical_analyzer.c
 * @brief This file implements interface for communication with lexical analyzer
 * @author Rastislav Budinsky
 */
#include <string.h>
#include <stdlib.h>
#include "Lexical_analyzer.h"
#include "Lexical_FSM.h"

#define KEYWORDS_SUM 15
char *keywords[] = {"do", "else", "end", "function", "global", "if", "integer", "local", "nil", "number", "require", "return", "string", "then", "while"};

/**
 * @brief decides whether the lexeme is a keyword
 * @param lexeme pointer to the lexem struct we want to check
 * @return returns 1 upon keyword, else 0
 */
int IsKeyWord(LexicalOutput *lexeme)
{
    for (int i = 0; i < KEYWORDS_SUM; i++)
    {
        if (strcmp(getString(lexeme), keywords[i]) == 0)
            return 1;
    }

    return 0;
}

Token *getToken(FILE *input)
{
    Token *token = (Token*) malloc(sizeof(Token));

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