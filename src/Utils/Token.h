/**
 * @file Token.h
 * @brief This file contains struct Token and enum for the struct
 * @author Rastislav Budinsky
 */
#ifndef TOKEN_H
#define TOKEN_H

/**
 * enum for classification of Token
 */
typedef enum {ERROR, ID, KEYWORD, FLOAT, STRING, MINUS, PLUS, MUL, DIV, INT_DIV, DEF, COMMA, LEN, LEFT, RIGHT, LESS, LEQ, GRT, GEQ, CONCAT, ASS, EQ, NEQ} TokenType;

/**
 * class defining Token for output communication from Lexical analyzer
 */
typedef struct {
    TokenType type;
    void *attribute;
} Token;

#endif // !TOKEN_H