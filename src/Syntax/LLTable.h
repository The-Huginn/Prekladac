/**
 * @file LLTable.h
 * @brief This file contains interface for working with LLTable and LLTable itself
 * @author Rastislav Budinsky
 */
#ifndef __LLTABLE_H__
#define __LLTABLE_H__

#include <stdbool.h>

typedef struct
{
    bool isTerminal;
    int value;
}Symbol;

/**
 * @brief Initializes Symbol
 * @param isTerminal set true if terminal otherwise false
 * @param value the enum value
 * @return pointer to the newly created Symbol, might return NULL
 */
Symbol *Symbol_Init(bool isTerminal, int value);

/**
 * @brief destroys Symbol struct
 * @param symbol
 */
void Symbol_Destroy(Symbol* symbol);

/**
 * @param symbol
 * @return true if terminal otherwise false
 */
bool Symbol_IsTerminal(Symbol *symbol);

/**
 * @param symbol
 * @return the int value of enum
 */
int Symbol_getValue(Symbol *symbol);




#endif