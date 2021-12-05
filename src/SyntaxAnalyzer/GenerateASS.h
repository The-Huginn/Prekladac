// IFJ Project 2021
/**
 * @file GenerateASS.h
 * @author Rastislav Budinsky (xbudin05)
 * @brief This file contains Abstract-Syntax-Tree functions @note function do not return anything so unmalloced memory can't be forwarded
 */
#ifndef __GENERATE_ASS_H__
#define __GENERATE_ASS_H__

#include "../Utils/Buffers.h"
#include "../Utils/node.h"
#include "../Utils/symtable.h"

/**
 * @brief Generates code for the header of the file
 * @param buffer Buffers
 */
void ASS_AddHeader(Buffers *buffer);

/**
 * @brief Generates code for the footer of the file
 * @param buffer Buffers
 */
void ASS_AddFooter(Buffers *buffer);

/**
 * @brief Adds code-branch for if condition and creates new scope in symtable
 *      @note only ASS_AddElseif, ASS_AddElse and ASS_PopEnd are expected calls
 * @param buffer Buffers
 * @param expression Node
 * @param symtable Symtable
 * @return -1 upon success otherwise error code
 */
int ASS_AddCondition(Buffers *buffer, Node *expression, Symtable *symtable);

/**
 * @brief Adds elseif code-branch on top of current if condition code-branch and creates new scope in symtable
 * @param buffer Buffers
 * @param expression Node
 * @param symtable Symtable
 * @return -1 upon success otherwise error code
 */
int ASS_AddElseif(Buffers *buffer, Node *expression, Symtable *symtable);

/**
 * @brief Adds else code-branch on top of current if condition code-branch and creates new scope in symtable
 * @param buffer Buffers
 * @param symtable Symtable
 * @return -1 upon success otherwise error code
 */
int ASS_AddElse(Buffers *buffer, Symtable *symtable);

/**
 * @brief Adds while code-branch for while loop and creates new scope in symtable
 * @param buffer Buffers
 * @param symtable Symtable
 * @return -1 upon success otherwise error code
 */
int ASS_AddWhile(Buffers *buffer, Symtable *symtable);

/**
 * @brief Adds for code-branch for for loop and creates new scope in symtable
 * @param buffer Buffers
 * @return -1 upon success otherwise error code
 */
int ASS_AddFor(Buffers *buffer);

/**
 * @brief Adds repeat code-branch for repeat-until loop and creates new scope in symtable
 * @param buffer Buffers
 * @param symtable Symtable
 * @return -1 upon success otherwise error code
 */
int ASS_AddRepeat(Buffers *buffer, Symtable *symtable);

/**
 * @brief Adds break code-branch to break out of most-inner loop
 * @param buffer Buffers
 * @return -1 upon success otherwise error code
 */
int ASS_AddBreak(Buffers *buffer);

/**
 * @brief Adds function definition code-branch generates frame&pops values
 * @param buffer Buffers
 * @return -1 upon success otherwise error code
 */
int ASS_AddFunction(Buffers *buffer);

/**
 * @brief Generates code for declaration of variables @note variables are expected to be in buffer->variables, this Vector is not changed
 * @param buffer Buffers
 */
void ASS_DeclareVariables(Buffers *buffer);

/**
 * @brief Generates code for assignment of expressions into variables @note variables and expressions are expected in buffer->variables, buffer->expressions respectively. Vectors are not changed
 * @param buffer Buffers
 */
void ASS_GenerateAssign(Buffers *buffer);

/**
 * @brief Generates code for function call @note function expected in buffer->variables, arguments in buffer->expressions. @note Vectors are not changed. Return values are not used
 * @param buffer Buffers
 * @param function_call Node with function call expression
 */
void ASS_GenerateFunctionCall(Buffers *buffer, Node *function_call);

/**
 * @brief Generates code for function return @note function expected in buffer->current_function and return values in buffer->expressions. Vectors are not changed.
 * @param buffer Buffers
 */
void ASS_GenerateFunctionReturn(Buffers *buffer);

/**
 * @brief This function should be called upon found 'end' terminal or after reading 'until <condition>' and removes last scope from symtable
 * @param buffer Buffers
 * @param symtable Symtable
 * @return -1 upon success otherwise error code
 */
int ASS_PopEnd(Buffers *buffer, Symtable *symtable);

#endif // !__GENERATE_ASS_H__