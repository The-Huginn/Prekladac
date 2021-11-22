/**
 * @file SemanticActions.h
 * @author Rastislav Budinsky
 * @brief This file contains function for semantic actions and supportive functions
 */
#ifndef __SEMANTIC_ACTIONS_H__
#define __SEMANTIC_ACTIONS_H__

#include <stdbool.h>

#include "../Utils/node.h"
#include "../Utils/vector.h"

/**
 * @brief This function only parses one expressions to multiple expressions and removes expressions with comma
 * @param root Node, which children are parsed
 * @return true upon success otherwise false
 */
bool AbstractSemanticTree_UpdateFunctionCalls(Node *root);

/**
 * @brief This functions extends last parameter if it is function call and not enough params. Check whether Semantic action is alright, matches function signature.
 *      @note Before this function should be called @see AbstractSemanticTree_UpdateFunctionCalls to remove Void parameter
 *      @note Changes Semantic type of function Node
 * @param root Node of the function call
 * @return -1 if success otherwise semantic error
 */
int AbstractSemanticTree_VerifyFunctionCall(Node *root);

/**
 * @brief This function check whether the Binary operator has valid parameters
 *      @note Changes Semantic type of Node
 * @param root Node of Binary operator
 * @return -1 if success otherwise semantic error
 */
int AbstractSemanticTree_BinaryOperator(Node *root);

/**
 * @brief This function check whether the Unary operator has valid parameters
 *      @note Changes Semantic type of Node
 * @param root Node of Unary operator
 * @return -1 if success otherwise semantic error
 */
int AbstractSemanticTree_UnaryOperator(Node *root);

#endif // !__SEMANTIC_ACTIONS_H__