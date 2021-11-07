/**
 * @file LLTable.h
 * @brief This file contains interface for working with LLTable and LLTable itself
 * @note Rules are already in reverse order, can be pushed to stack
 * @author Rastislav Budinsky
 */
#ifndef __LLTABLE_H__
#define __LLTABLE_H__

#include <stdbool.h>

#define LLTABLE_WIDTH 46
#define LLTABLE_HEIGHT 41
#define RULE_ARRAY_SIZE 89

typedef struct
{
    bool isTerminal;
    int value;
}Symbol;

typedef struct
{
    int size;
    Symbol left_side;
    Symbol right_side[10] ;
}Rule;

extern int LLTable[LLTABLE_HEIGHT][LLTABLE_WIDTH]; //! 2D LLTable with indexes to the Rule

extern Rule rules[RULE_ARRAY_SIZE];    //! array of Rules

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
int Symbol_GetValue(Symbol *symbol);

/**
 * @param rule
 * @return size of the rule
 */
int Rule_GetSize(Rule *rule);

/**
 * @param rule
 * @param index index of the symbol
 * @return symbol at specific place
 */
Symbol *Rule_GetSymbol(Rule *rule, int index);

#endif
