/**
 * @file node.h
 * @author Rastislav Budinsky
 * @brief This file contains struct Node and interface for working with this structure
 */
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
 * @param parent Node
 * @return Vector of sons
 */
Vector *Node_GetSons(Node *parent);

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
