/**
 * @file Token.h
 * @brief This file contains struct Token and enum for the struct
 * @author Rastislav Budinsky
 */
#ifndef TOKEN_H
#define TOKEN_H

#include"../Syntax/Terminal.h"

/**
 * class defining Token for output communication from Lexical analyzer
 */
typedef struct {
    Terminal type;
    void *attribute;
} Token;

#endif // !TOKEN_H