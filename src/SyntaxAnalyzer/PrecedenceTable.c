/**
 * @file PrecedenceTable.c
 * @author Rastislav Budinsky
 * @brief This file implements the PrecedenceTable @note upon changes should change macros in PrecedenceTable.h
 */
#include "PrecedenceTable.h"

#include <stdlib.h>

char precedenceTable[PRECEDENCE_TABLE_HEIGHT][PRECEDENCE_TABLE_WIDTH] = {
{'<' , '=' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , 'X' , 'X'},
{'X' , '>' , 'X' , 'X' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , 'X' , 'X' , '>'},
{'<' , '>' , 'X' , 'X' , '>' , '>' , '>' , '>' , '>' , 'X' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , 'X' , '>'},
{'<' , '>' , 'X' , 'X' , 'X' , 'X' , 'X' , 'X' , 'X' , 'X' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , 'X' , '>'},
{'<' , '>' , '<' , 'X' , '>' , '>' , '>' , '>' , '>' , 'X' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , 'X' , '>'},
{'<' , '>' , '<' , 'X' , '>' , '>' , '>' , '>' , '>' , 'X' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , 'X' , '>'},
{'<' , '>' , '<' , 'X' , '>' , '>' , '>' , '>' , '>' , 'X' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , 'X' , '>'},
{'<' , '>' , '<' , 'X' , '<' , '<' , '<' , '>' , '>' , 'X' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , 'X' , '>'},
{'<' , '>' , '<' , 'X' , '<' , '<' , '<' , '>' , '>' , 'X' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , 'X' , '>'},
{'<' , '>' , 'X' , 'X' , 'X' , 'X' , 'X' , 'X' , 'X' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , 'X' , '>'},
{'<' , '>' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , 'X' , '>'},
{'<' , '>' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , 'X' , '>'},
{'<' , '>' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , 'X' , '>'},
{'<' , '>' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , 'X' , '>'},
{'<' , '>' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , 'X' , '>'},
{'<' , '>' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , 'X' , '>'},
{'<' , '>' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '<' , 'X' , '>'},
{'<' , '>' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '<' , 'X' , '>'},
{'a' , '>' , 'X' , 'X' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , 'X' , 'X' , '>'},
{'X' , '>' , 'X' , 'X' , 'X' , 'X' , 'X' , 'X' , 'X' , 'X' , 'X' , 'X' , 'X' , 'X' , 'X' , 'X' , 'X' , 'X' , 'X' , '>' , 'X'},
{'b' , '=' , 'b' , 'b' , 'b' , 'b' , 'b' , 'b' , 'b' , 'b' , 'b' , 'b' , 'b' , 'b' , 'b' , 'b' , 'b' , 'b' , 'b' , 'c' , 'X'},
{'<' , 'X' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , 'X' , 'X' }};

Precedence_Rule precedence_rules[PRECEDENCE_RULE_ARRAY_SIZE] = {
{.size = 1, .right_side = { {.type = P_I} } },
{.size = 1, .right_side = { {.type = P_VOID} } },
{.size = 2, .right_side = { {.type = P_E}, { .type = P_UNARY_OPERATOR} } },
{.size = 3, .right_side = { {.type = P_E}, {.type = P_BINARY_OPERATOR}, {.type = P_E} } },
{.size = 3, .right_side = { {.type = P_RIGHT}, {.type = P_E}, {.type = P_LEFT} } },
{.size = 4, .right_side = { {.type = P_RIGHT}, {.type = P_E}, {.type = P_FUNCTION}, {.type = P_I} } },
{.size = 3, .right_side = { {.type = P_E}, {.type = P_COMMA}, {.type = P_E} } } 
};


PrecedenceItem *PrecedenceItem_Init(PrecedenceItemType type, char character)
{
    PrecedenceItem *item = (PrecedenceItem *) malloc(sizeof(PrecedenceItem));
    if (item == NULL)
        return NULL;

    item->type = type;
    item->character = character;

    return item;
}

void PrecedenceItem_Destroy(PrecedenceItem *item)
{
    free(item);
}

PrecedenceItemType PrecedenceItem_GetType(PrecedenceItem *item)
{
    return item->type;
}

char PrecedenceItem_GetChar(PrecedenceItem *item)
{
    return item->character;
}

void PrecedenceItem_SetChar(PrecedenceItem *item, char character)
{
    item->character = character;
}

PrecedenceItemType PrecedenceItem_IsBinaryOperator(PrecedenceItem *item)
{
    return
        (item->type == P_MUL ||
        item->type == P_DIV ||
        item->type == P_INT_DIV ||
        item->type == P_PLUS ||
        item->type == P_MINUS ||
        item->type == P_CONCAT ||
        item->type == P_GRT ||
        item->type == P_LESS ||
        item->type == P_GEQ ||
        item->type == P_LEQ ||
        item->type == P_EQ ||
        item->type == P_NEQ ||
        item->type == P_AND ||
        item->type == P_OR)

        ? P_BINARY_OPERATOR : P_VOID;
}

PrecedenceItemType PrecedenceItem_IsUnaryOperator(PrecedenceItem *item)
{
    return
        (item->type == P_LEN ||
        item->type == P_NOT)

        ? P_UNARY_OPERATOR : P_VOID;
}

PrecedenceItemType Token_ToPrecedenceItemType(Token *token)
{
    Terminal token_type = Token_getType(token);

    switch (token_type)
    {
    case T_COMMA:
       return P_COMMA;
    case T_LEFT:
        return P_LEFT;
    case T_RIGHT:
        return P_RIGHT;
    case T_LEN:
        return P_LEN;
    case K_NOT:
        return P_NOT;
    case T_MUL:
        return P_MUL;
    case T_DIV:
        return P_DIV;
    case T_INT_DIV:
        return P_INT_DIV;
    case T_PLUS:
        return P_PLUS;
    case T_MINUS:
        return P_MINUS;
    case T_CONCAT:
        return P_CONCAT;
    case T_GRT:
        return P_GRT;
    case T_LESS:
        return P_LESS;
    case T_GEQ:
        return P_GEQ;
    case T_LEQ:
        return P_LEQ;
    case T_EQ:
        return P_EQ;
    case T_NEQ:
        return P_NEQ;
    case K_AND:
        return P_AND;
    case K_OR:
        return P_OR;
    case T_STRING:
    case T_NUMBER:
    case T_INTEGER:
    case T_ID:
    case K_TRUE:
    case K_FALSE:
    case K_NIL:
        return P_I;

    
    }

    return P_$;
}
