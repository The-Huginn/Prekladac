/**
 * @file Lexical_analyzer.h
 * @brief This file implements interface for communication with the Lexical analyzer
 * @author Rastislav Budinsky
 */
#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include "../Utils/Token.h"

/**
 * @brief function finds next valid lexeme
 * @return struct Token containing info about the found lexeme or error
 */
Token *getToken();


#endif // !LEXICAL_ANALYZER_H