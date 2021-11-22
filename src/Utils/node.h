/**
 * @file node.h
 * @author Rastislav Budinsky
 * @brief This file contains struct Node and interface for working with this structure
 */
#ifndef __NODE_H__
#define __NODE_H__

#include "vector.h"
#include "semantictype.h"
#include "../SyntaxAnalyzer/PrecedenceTable.h"

#include <stdbool.h>

typedef enum {NODE_OPERATION, NODE_ID, NODE_FUNCTION, NODE_VALUE, NODE_VOID}NodeType;

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
 * @brief Destroys Node
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
 * @return Vector of identifiers for retrieving values of expressions @note Vector is needed for function return
 */
Vector* Node_PostOrder(Node* node, bool destroy);

#endif //!__NODE_H__