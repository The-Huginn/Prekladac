/**
 * @file BottomToTop.h
 * @author Rastislav Budinsky
 * @brief This file contains interface for calling BottomToTop parser
 */
#ifndef __BOTTOM_TO_TOP_H__
#define __BOTTOM_TO_TOP_H__
#include "PrecedenceTable.h"
#include "../Utils/node.h"
#include "../Utils/symtable.h"
#include <stdio.h>

/**
 * @brief Parsing method for parsing expressions only.
 * @param input Input file
 * @param output Output file
 * @param error_output Output for writing program warnings etc.
 * @param expression Address of pointer where parsed expression is saved to. Undefined upon error code returned.
 * @return -1 upon success otherwise code of error
 */
int BottomToTop(FILE* input, FILE* output, FILE* error_output, Node** expression, Symtable *symtable);

#endif // !__BOTTOM_TO_TOP_H__