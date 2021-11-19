/**
 * @file Token.c
 * @brief This file implements interface for Token struct
 * @author Rastislav Budinsky
 */
#include "Token.h"

Terminal Token_getType(Token *token)
{
    return token->type;
}

void *Token_getData(Token *token)
{
    return token->attribute;
}