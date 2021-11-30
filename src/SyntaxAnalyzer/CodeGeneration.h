/**
 * @file CodeGeneration.h
 * @author Rastislav Budinsky
 * @brief This file contains code-generating functions @note function do not return anything so unmalloced memory can't be forwarded
 */
#ifndef __CODE_GENERATION_H__
#define __CODE_GENERATION_H__

#include "../Utils/Buffers.h"
#include "../Utils/node.h"
#include "../Utils/symtable.h"

/**
 * @brief Generates code for the header of file
 * @param buffer Buffers
 */
void Code_AddHeader(Buffers *buffer);

/**
 * @brief Adds code-branch for if condition and creates new scope in symtable
 *      @note only Code_AddElseif, Code_AddElse and Code_PopEnd are expected calls
 * @param buffer Buffers
 * @param expression Node
 * @param symtable Symtable
 * @return -1 upon success otherwise error code
 */
int Code_AddCondition(Buffers *buffer, Node *expression, Symtable *symtable);

/**
 * @brief Adds elseif code-branch on top of current if condition code-branch and creates new scope in symtable
 * @param buffer Buffers
 * @param expression Node
 * @param symtable Symtable
 * @return -1 upon success otherwise error code
 */
int Code_AddElseif(Buffers *buffer, Node *expression, Symtable *symtable);

/**
 * @brief Adds else code-branch on top of current if condition code-branch and creates new scope in symtable
 * @param buffer Buffers
 * @param symtable Symtable
 * @return -1 upon success otherwise error code
 */
int Code_AddElse(Buffers *buffer, Symtable *symtable);

/**
 * @brief Adds while code-branch for while loop and creates new scope in symtable
 * @param buffer Buffers
 * @param expression Node
 * @param symtable Symtable
 * @return -1 upon success otherwise error code
 */
int Code_AddWhile(Buffers *buffer, Node *expression, Symtable *symtable);

/**
 * @brief Adds function definition code-branch generates frame&pops values
 * @param buffer Buffers
 * @return -1 upon success otherwise error code
 */
int Code_AddFunction(Buffers *buffer);

/**
 * @brief Generates code for declaration of variables @note variables are expected to be in buffer->variables, this Vector is not changed
 * @param buffer Buffers
 */
void Code_DeclareVariables(Buffers *buffer);

/**
 * @brief Generates code for assignment of expressions into variables @note variables and expressions are expected in buffer->variables, buffer->expressions respectively. Vectors are not changed
 * @param buffer Buffers
 */
void Code_GenerateAssign(Buffers *buffer);

/**
 * @brief Generates code for function call @note function expected in buffer->variables, arguments in buffer->expressions. @note Vectors are not changed. Return values are not used
 * @param buffer Buffers
 * @param function_call Node with function call expression
 */
void Code_GenerateFunctionCall(Buffers *buffer, Node *function_call);

/**
 * @brief Generates code for function return @note function expected in buffer->current_function and return values in buffer->expressions. Vectors are not changed.
 * @param buffer Buffers
 */
void Code_GenerateFunctionReturn(Buffers *buffer);

/**
 * @brief This function should be called upon found 'end' terminal and removes last scope from symtable
 * @param buffer Buffers
 * @param symtable Symtable
 */
void Code_PopEnd(Buffers *buffer, Symtable *symtable);

#endif // !__CODE_GENERATION_H__