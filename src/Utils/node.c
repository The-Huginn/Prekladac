/**
 * @file node.c
 * @author Rastislav Budinsky
 * @brief This file implements struct Node and it's interface
 */
#include "node.h"
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

void DataDtor(void *data)
{
    free(data);
}

void *CreateNumber(int number)
{
    void *createdNumber = (void*)malloc(sizeof(int));
    if (createdNumber == NULL)
        return NULL;

    *((int*)createdNumber) = number;

    return createdNumber;
}

Vector* Node_PostOrder(Node *node, bool destroy)
{
    for (int i = 0; i < Vector_Size(node->sons); i++)
    {
        Vector *returned_values = Node_PostOrder(Vector_GetElement(node->sons, i), destroy);
        if (returned_values == NULL)
            break;
        
        // TODO
        Vector_Destroy(returned_values);
    }

    if (destroy)
        Node_Destroy(node, false);

    // TODO
    Vector *return_values = Vector_Init(DataDtor);
    if (return_values == NULL)
        return NULL;

    // add to return_values int values where data is saved can use CreateNumber function
    
    return return_values;
}