/**
 * @file Token.c
 * @brief This file implements interface for Token struct
 * @author Rastislav Budinsky
 */
#include "Token.h"

Terminal getType(Token *token)
{
    return token->type;
}

void *getData(Token *token)
{
    return token->attribute;
}