/**
 * @file Token.h
 * @brief This file contains struct Token and enum for the struct
 * @author Rastislav Budinsky
 */
#ifndef TOKEN_H
#define TOKEN_H

#include"../SyntaxAnalyzer/Terminal.h"

/**
 * class defining Token for output communication from Lexical analyzer
 */
typedef struct {
    Terminal type;
    void *attribute;
} Token;

/**
 * @param token
 * @return enum type of token
 */
Terminal Token_getType(Token *token);

/**
 * @param token
 * @return data of token
 */
void *Token_getData(Token *token);

/**
 * @brief Destroys token and attribute
 * @param token Token
 */
void Token_Destroy(Token *token);

#endif // !TOKEN_H