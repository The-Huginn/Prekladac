/**
 * @file CodeGeneration.h
 * @author Rastislav Budinsky
 * @brief This file contains code-generating functions @note function do not return anything so unmalloced memory can't be forwarded
 */
#ifndef __CODE_GENERATION_H__
#define __CODE_GENERATION_H__

#include "../Utils/Buffers.h"
#include "../Utils/node.h"

/**
 * @brief Generates code for the header of file
 * @param buffer Buffers
 */
void Code_AddHeader(Buffers *buffer);

/**
 * @brief Adds code-branch for if condition
 *      @note only Code_AddElseif, Code_AddElse and Code_PopEnd are expected calls
 * @param buffer Buffers
 * @param expression Node
 */
void Code_AddCondition(Buffers *buffer, Node *expression);

/**
 * @brief Adds elseif code-branch on top of current if condition code-branch
 * @param buffer Buffers
 * @param expression Node
 */
void Code_AddElseif(Buffers *buffer, Node *expression);

/**
 * @brief Adds else code-branch on top of current if condition code-branch
 * @param buffer Buffers
 */
void Code_AddElse(Buffers *buffer);

/**
 * @brief Adds while code-branch for while loop
 * @param buffer Buffers
 * @param expression Node
 */
void Code_AddWhile(Buffers *buffer, Node *expression);

/**
 * @brief Adds function definition code-branch generates frame&pops values
 * @param buffer Buffers
 */
void Code_AddFunction(Buffers *buffer);

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
 * @brief This function should be called upon found 'end' terminal
 * @param buffer Buffers
 */
void Code_PopEnd(Buffers *buffer);

#endif // !__CODE_GENERATION_H__