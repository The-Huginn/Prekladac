/**
 * @file node.c
 * @author Rastislav Budinsky
 * @brief This file implements struct Node and it's interface
 */
#include "node.h"
#include "vector.h"
#include "logger.h"

#include <stdlib.h>

struct Node_t
{
    NodeType nodeType;
    void *data;
    Vector *sons;
    SemanticType semanticType;
    void (*DataDtor)(void*);
};

Node *Node_Init(NodeType nodeType, void *data, SemanticType semanticType, void (*DataDtor)(void*))
{
    Node *node = (Node*) malloc(sizeof(Node));
    if (node == NULL)
        ERROR("Allocation failed");

    node->sons = Vector_Init(NULL);
    if (node->sons)
    {
        free(node);
        ERROR("Allocation failed");
    }

    node->nodeType = nodeType;
    node->data = data;
    node->semanticType = semanticType;
    node->DataDtor = DataDtor;
}

void Node_Destroy(Node *node, bool destroy)
{
    if (destroy)
        for (int i = 0; i < Vector_Size(node->sons); i++)
            Node_Destroy(Vector_GetElement(node->sons, i), true);

    Vector_Destroy(node->sons);
    free(node);
}

bool Node_AppendSon(Node *node, Node *son)
{
    return Vector_PushBack(node->sons, son);
}

int Node_PostOrder(Node *node, bool destroy)
{
    for (int i = 0; i < Vector_Size(node->sons); i++)
        Node_PostOrder(Vector_GetElement(node->sons, i), destroy);

    if (destroy)
        Node_Destroy(node, false);

    // TODO
    return 0;
}