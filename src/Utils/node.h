/**
 * @file node.h
 * @author Rastislav Budinsky
 * @brief This file contains struct Node and interface for working with this structure
 */
#ifndef __NODE_H__
#define __NODE_H__

#include "vector.h"
#include "semantictype.h"
#include "Buffers.h"
#include "../SyntaxAnalyzer/PrecedenceTable.h"

#include <stdbool.h>
#include <stdio.h>

typedef enum {NODE_OPERATION, NODE_ID, NODE_FUNCTION, NODE_VALUE, NODE_VOID, NODE_NIL, NODE_POP}NodeType;

#define TMP(a) "$tmp", (a)
#define ELEMENT(a) Element_GetKey((Element*)(a)), Element_GetID((Element*)(a))

typedef struct Node_t Node;

/**
 * @param nodeType NodeType
 * @param data Data
 * @param semanticType SemanticType
 * @param DataDtor Desctructor for Data
 * @param operation If NodeType equals NODE_OPERATION then sets remembers operation otherwise it is set to P_VOID
 */
Node *Node_Init(NodeType NodeType, void *data, SemanticType semanticType, void (*DataDtor)(void *), PrecedenceItemType operation);

/**
 * @brief New node is created (pointer is preserved) and this node compares previous old Node with nil
 *      @note Pointer is preserved and original Node is being pointed to by the same pointer
 * @param old Node
 * @return true upon success otherwise false
 */
bool Node_CompareWithNil(Node *old);

/**
 * @brief Destroys Node and it's returns
 * @param node Node
 * @param destroy To destroy all sons as well
 */
void Node_Destroy(Node* node, bool destroy);

/**
 * @brief Appends new son to parent node
 * @param parent 
 * @param son
 * @return True upon success otherwise false
 */
bool Node_AppendSon(Node* parent, Node *son);

/**
 * @brief Appends new return to parent node
 * @param parent 
 * @param semanticType 
 * @return True upon success otherwise false
 */
bool Node_AppendReturn(Node *parent, SemanticType semanticType);

/**
 * @param node Node
 * @return data
 */
void *Node_GetData(Node *node);

/**
 * @brief Sets how many times use the last expression @note It is expected this parameter is function call and has enough return values, otherwise unexpected behaviour
 * @param node Function call
 * @param count How many times use the last param explicitly @note It is already by default used once, default value is 0
 */
void Node_SetParamCount(Node *node, int count);

/**
 * @brief Getter for @see Node_SetParamCount
 * @param node Function call
 * @return default 0 unless change by @see Node_SetParamCount
 */
int Node_GetParamCount(Node *node);

/**
 * @param parent Node
 * @return Vector of sons
 */
Vector *Node_GetSons(Node *parent);

/**
 * @param parent 
 * @return Vector of returns 
 */
Vector *Node_GetReturns(Node *parent);

/**
 * @param root Node
 * @return Semantic type of Node 
 */
SemanticType Node_GetSemantic(Node *root);

/**
 * @param root Node
 * @param semanticType new SemanticType
 */
void Node_SetSemantic(Node *root, SemanticType semanticType);

/**
 * @param root Node
 * @return Type of Node 
 */
NodeType Node_GetType(Node *root);

/**
 * @param node Node
 * @return If Node is type of operation returns Operation otherwise P_VOID
 */
PrecedenceItemType Node_GetOperation(Node *node);

/**
 * @param node 
 * @return True if Node is operation otherwise false
 */
bool Node_IsOperation(Node *node);

/**
 * @brief Iterates in post-order for code-generation
 * @param node Node
 * @param destroy Set to true to call destructor upon all Nodes
 * @param buffer Buffers for changing offset @note function changes content of buffer->tmp_offset
 * @param expected_amount The expected amount of returned variables
 * @return Vector of identifiers for retrieving values of expressions @note Vector is needed for function return
 */
Vector* Node_PostOrder(Node* node, bool destroy, Buffers *buffer, int expected_amount);

/**
 * @brief Malloc int
 * @param number Int value
 * @return Pointer to int
 */
void *Number_Init(int number);

/**
 * @brief Destroys Number
 * @param data Int value on heap
 */
void Number_Destroy(void *data);

#endif //!__NODE_H__