/**
 * @file LexicalAnalyzer.h
 * @brief This file implements interface for communication with the Lexical analyzer
 * @author Rastislav Budinsky
 */
#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <stdio.h>
#include "../Utils/Token.h"

/**
 * @brief checks, if any tokens were returned, otherwise finds next valid lexeme
 * @param file stream to read from
 * @return struct Token containing info about the found lexeme or error
 */
Token *getToken(FILE* file);

/**
 * @brief previously gotten token can be returned to stack, should return from the newest one
 * @param token
 */
void returnToken(Token *token);

/**
 * @brief should be called when program exiting, to destroy stack
 */
void LexicalDestroy();


#endif // !LEXICAL_ANALYZER_H