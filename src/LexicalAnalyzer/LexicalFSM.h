// IFJ Project 2021
/**
 * @file LexicalFSM.h
 * @brief This file contains struct LexicalOutput and enum for every state of the FSM
 * contains one function for finding one valid lexeme and one for getting Lexeme length
 * @note LexicalType final state enums must preserve ordered structure to correspond with Syntactic Analyzer
 * @author Rastislav Budinsky (xbudin05)
 */
#ifndef LEXICAL_FSM_H
#define LEXICAL_FSM_H

#include <stdio.h>
#include <stdbool.h>
#include "../Utils/Token.h"

#define MAX_LEXEME_LEN 256

// update Lexical_FSM.c LexicalOutput_IsFinal function upon changes
typedef enum {START, LOAD_ID, LOAD_INT_INT_PART, POINT, EXPONENT, SIGN, EXP_PART, LOAD_STRING, STRING_FINALIZE, ESC_SEQ, LIMIT_2, LIMIT_5, NORMAL_1, NORMAL_2,
    PLUS_FINALIZE, MUL_FINALIZE, DIV_STATE_INT_DIV_STATE, INT_DIV_FINALIZE, DEF_FINALIZE, COMMA_FINALIZE, LEN_FINALIZE, LEFT_FINALIZE, RIGHT_FINALIZE,
    LESS_STATE_LEQ_STATE, LEQ_FINALIZE, GRT_STATE_GEQ_STATE, GEQ_FINALIZE, CONCAT_STATE, CONCAT_FINALIZE, ASS_STATE_EQ_STATE, EQ_FINALIZE, NEQ_STATE, NEQ_FINALIZE,
    MINUS_EN_DASH, EM_DASH, LINE_COM, EXP_BLOCK, BLOCK_COM, END_BLOCK,
    ERROR_STATE, EOF_STATE, F_ASS, F_COMMA, F_CONCAT, F_DEF, F_DIV, F_EQ, F_GEQ, F_GRT, F_ID, F_INT_DIV, F_INTEGER, F_LEFT, F_LEN, F_LEQ, F_LESS, F_MINUS, F_MUL, F_NEQ, F_NUMBER, F_PLUS, F_RIGHT, F_STRING
    }LexicalType;

typedef struct {
    LexicalType state;
    char lexeme[MAX_LEXEME_LEN];
    short pos;
    bool esc_seq;
} LexicalOutput;

/**
 * @brief finds first valid lexeme according to the defined FSM, later user should free after use
 * @param input input stream file descriptor
 * @return pointer to newly created struct containing info about FSM output
 */
LexicalOutput *getLexeme(FILE *input);

/**
 * @brief returns lexeme string, should end with '\0'
 * @param lexeme pointer to the lexeme we want to get
 * @return pointer to the beginning of string
 */
char* getString(LexicalOutput *lexeme);

/**
 * @brief should be called to destroy lexem struct
 * @param lexeme pointer to the lexeme we want to destroy
 */
void freeLexeme(LexicalOutput *lexeme);

/**
 * @brief get final state of FSM
 * @param lexeme pointer to the lexeme we want to know final state of
 * @return enum value of final state
 */
LexicalType getFinalState(LexicalOutput *lexeme);

#endif // !LEXICAL_FSM_H