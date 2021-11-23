/**
 * @file Token.c
 * @brief This file implements interface for Token struct
 * @author Rastislav Budinsky
 */
#include "Token.h"

#include <stdlib.h>

Terminal Token_getType(Token *token)
{
    return token->type;
}

void *Token_getData(Token *token)
{
    return token->attribute;
}

void Token_Destroy(Token *token)
{
    // need to clear buffer
    if (token->type == T_ID ||
        token->type == T_INTEGER ||
        token->type == T_NUMBER ||
        token->type == T_STRING)
        free(token->attribute);

    free(token);
}