/**
 * @file PrecedenceTable.h
 * @author Rastislav Budinsky
 * @brief This file contains the declaration of precedence table
 */
#ifndef __PRECEDENCE_TABLE_H__
#define __PRECEDENCE_TABLE_H__

#define PRECEDENCE_TABLE_HEIGHT 20
#define PRECEDENCE_TABLE_WIDTH (PRECEDENCE_TABLE_HEIGHT)
#define PRECEDENCE_RULE_ARRAY_SIZE 4

#include <stdbool.h>
#include "../Utils/Token.h"

typedef enum {
    P_VOID, P_COMMA, P_E, P_BINARY_OPERATOR, P_UNARY_OPERATOR, P_LEFT, P_RIGHT, P_LEN, P_NOT, P_MUL, P_DIV, P_INT_DIV, P_PLUS, P_MINUS, P_CONCAT, P_GRT, P_LESS, P_GEQ, P_LEQ, P_EQ, P_NEQ, P_AND, P_OR, P_I, P_$
}PrecedenceItemType;

typedef struct
{
    PrecedenceItemType type;
    char character;
}PrecedenceItem;

typedef struct
{
    int size;
    PrecedenceItem right_side[5];
}Precedence_Rule;

extern char precedenceTable[PRECEDENCE_TABLE_HEIGHT][PRECEDENCE_TABLE_WIDTH];

extern Precedence_Rule precedence_rules[PRECEDENCE_RULE_ARRAY_SIZE];

/**
 * @brief Initializes PrecedenceItem
 * @param type type of PrecedenceItem
 * @param character char of PrecedenceItem
 * @return PrecedenceItem* 
 */
PrecedenceItem *PrecedenceItem_Init(PrecedenceItemType type, char character);

/**
 * @brief destroys PrecedenceItem
 * @param item PrecedenceItem
 */
void PrecedenceItem_Destroy(PrecedenceItem *item);

/**
 * @param item PrecedenceItem
 * @return type of PrecedenceItem
 */
PrecedenceItemType PrecedenceItem_GetType(PrecedenceItem *item);

/**
 * @param item  PrecedenceItem
 * @return char of PrecedenceItem
 */
char PrecedenceItem_GetChar(PrecedenceItem *item);

/**
 * @param item PrecedenceItem
 * @return P_BINARY_OPERATOR if PrecedenceItem lies in BinaryOperator set otherwise P_VOID
 */
PrecedenceItemType PrecedenceItem_IsBinaryOperator(PrecedenceItem *item);

/**
 * @param item PrecedenceItem
 * @return P_UNARY_OPERATOR if PrecedenceItem lies in UnaryOperator set otherwise P_VOID
 */
PrecedenceItemType PrecedenceItem_IsUnaryOperator(PrecedenceItem *item);

/**
 * @param token Token
 * @return Token type converted to PrecedenceItemType, if no viable mapping option then P_ERROR
 */
PrecedenceItemType Token_ToPrecedenceItemType(Token *token);

#endif // !__PRECEDENCE_TABLE_H__