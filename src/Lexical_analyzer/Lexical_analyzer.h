/**
 * @file Lexical_analyzer.h
 * @brief This file implements interface for communication with the Lexical analyzer
 * @author Rastislav Budinsky
 */
#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <stdio.h>
#include "../Utils/Token.h"

/**
 * @brief function finds next valid lexeme
 * @param file stream to read from
 * @return struct Token containing info about the found lexeme or error
 */
Token *getToken(FILE* file);


#endif // !LEXICAL_ANALYZER_H