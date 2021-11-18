/**
 * @file node.h
 * @author Rastislav Budinsky
 * @brief This file contains struct Node and interface for working with this structure
 */
#include <stdbool.h>

typedef enum {NODE_OPERATION, NODE_ID, NODE_FUNCTION, NODE_VALUE, NODE_VOID}NodeType;

typedef enum {SEMANTIC_INTEGER, SEMANTIC_NUMBER, SEMANTIC_BOOLEAN, SEMANTIC_STRING, SEMANTIC_VOID}SemanticType;

typedef struct Node_t Node;

/**
 * @param nodeType NodeType
 * @param data Data
 * @param semanticType SemanticType
 * @param DataDtor Desctructor for Data
 */
Node *Node_Init(NodeType NodeType, void *data, SemanticType semanticType, void (*DataDtor)(void *));

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
 * @brief Iterates in post-order for code-generation
 * @param node Node
 * @param destroy Set to true to call destructor upon all Nodes
 * @return Identifier for value of evaluated expression
 */
int Node_PostOrder(Node* node, bool destroy);
