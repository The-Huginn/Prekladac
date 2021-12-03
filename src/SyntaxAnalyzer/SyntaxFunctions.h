/**
 * @file SyntaxFunctions.h
 * @author Rastislav Budinsky
 * @brief This file contains supportive functions for Syntax analyzer and for code generation
 */
#ifndef __SYNTAX_FUNCTIONS_H__
#define __SYNTAX_FUNCTIONS_H__

#include "../Utils/symtable.h"
#include "../Utils/Token.h"
#include "../Utils/vector.h"
#include "../Utils/node.h"
#include "../Utils/symbolelement.h"

typedef struct Buffers_t
{
    Vector *variables;
    Vector *expressions;
    Vector *only_declared;
    int position;
    bool declared;
    Element *current_function;
}Buffers;

/**
 * @brief Constructor for Buffers
 * @return Valid pointer upon success otherwise NULL
 */
Buffers *Buffers_Init();

/**
 * @brief Destructor for Buffers
 * @param buffer Buffers
 */
void Buffers_Destroy(Buffers *buffer);

/**
 * @param token Token
 * @return SemanticType of Token 
 */
SemanticType Syntax_GetSemantic(Token *token);

/**
 * @param token Token
 * @return True if token is of type Integer/Number/String/Boolean
 */
bool Syntax_IsDatatype(Token *token);

/**
 * @brief Adds new symbol to TS, checks for duplicates
 * @param symtable Symtable - TS
 * @param buffer Buffers
 * @param token Token
 * @param type SymbolType
 * @return -1 on success otherwise error code
 */
int Syntax_AddToTS(Symtable *symtable, Buffers *buffer, Token *token, SymbolType type);

/**
 * @brief Sets Semantic type to top variable
 * @param buffer Buffers
 * @param token Token
 * @return -1 upon success otherwise error code 
 */
int Syntax_Variable_SetSemantic(Buffers *buffer, Token *token);

/**
 * @brief Assigns all expressions to variables, missing expressions are filled with last function call or nils (depending on amount of returns from function) or removes extraneous
 * @param buffer Buffers
 * @return -1 upon success otherwise error code
 */
int Syntax_Variable_Assign(Buffers *buffer);

/**
 * @brief Assigns all expressions to returns of function missing expressions are filled with last function call of nils (depending on amount of returns from function) or removes extraneous
 * @param buffer Buffers
 * @return -1 upon success otherwise error code
 */
int Syntax_Return_Assign(Buffers *buffer);

/**
 * @brief Creates Node of function call and checks correctness of this function call
 * @param symtable Symtable
 * @param buffer Buffers
 * @return -1 upon success otherwise error code
 */
int Syntax_FunctionCall(Symtable *symtable, Buffers *buffer);

#endif //!__SYNTAX_FUNCTIONS_H__