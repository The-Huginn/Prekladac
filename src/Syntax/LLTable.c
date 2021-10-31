/**
 * @file LLTable.c
 * @brief This file implements interface of LLTable.h
 * @author Rastislav Budinsky
 */
#include "LLTable.h"
#include <stdlib.h>

Symbol *Symbol_Init(bool isTerminal, int value)
{
    Symbol *symbol = (Symbol*) malloc(sizeof(Symbol));
    if (symbol == NULL)
        return NULL;

    symbol->isTerminal = isTerminal;
    symbol->value = value;

    return symbol;
}

void Symbol_Destroy(Symbol* symbol)
{
    free(symbol);
}

bool Symbol_IsTerminal(Symbol *symbol)
{
    return symbol->isTerminal;
}

int Symbol_getValue(Symbol *symbol)
{
    return symbol->value;
}