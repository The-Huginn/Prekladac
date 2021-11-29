/**
 * @file node.c
 * @author Rastislav Budinsky
 * @brief This file implements struct Node and it's interface
 */
#include "node.h"
#include "logger.h"
#include "symbolelement.h"

#include <stdlib.h>

#define TMP "1tmp"

struct Node_t
{
    NodeType nodeType;
    void *data;
    Vector *sons;
    Vector *returns;
    SemanticType semanticType;
    int count;
    void (*DataDtor)(void*);
    PrecedenceItemType operation;    //! use only when it is operation
};

SemanticType Node_GetSemantic(Node *root)
{
    return root->semanticType;
}

void Node_SetSemantic(Node *root, SemanticType semanticType)
{
    root->semanticType = semanticType;
}

NodeType Node_GetType(Node *root)
{
    return root->nodeType;
}

PrecedenceItemType Node_GetOperation(Node *node)
{
    return node->nodeType == NODE_OPERATION ? node->operation : P_VOID;
}

bool Node_IsOperation(Node *node)
{
    return node->nodeType == NODE_OPERATION;
}

Node *Node_Init(NodeType nodeType, void *data, SemanticType semanticType, void (*DataDtor)(void*), PrecedenceItemType operation)
{
    Node *node = (Node*) malloc(sizeof(Node));
    if (node == NULL)
        ERROR("Allocation failed");

    node->sons = Vector_Init(NULL);
    if (node->sons == NULL)
    {
        free(node);
        ERROR("Allocation failed");
    }

    node->returns = Vector_Init((void(*)(void*))Node_Destroy);
    if (node->returns == NULL)
    {
        Vector_Destroy(node->sons);
        free(node);
        ERROR("Allocation failed");
    }

    node->nodeType = nodeType;
    node->data = data;
    node->semanticType = semanticType;
    node->count = 0;
    node->DataDtor = DataDtor;
    node->operation = nodeType == NODE_OPERATION ? operation : P_VOID;

    return node;
}

bool Node_CompareWithNil(Node *old)
{
    Node *node = (Node*) malloc(sizeof(Node));
    if (node == NULL)
        ERROR("Allocation failed");

    node->sons = Vector_Init(NULL);
    if (node->sons == NULL)
    {
        free(node);
        ERROR("Allocation failed");
    }

    node->returns = Vector_Init((void(*)(void*))Node_Destroy);
    if (node->returns == NULL)
    {
        Vector_Destroy(node->sons);
        free(node);
        ERROR("Allocation failed");
    }

    node->nodeType = old->nodeType;             old->nodeType = NODE_OPERATION;
    node->data = old->data;                     old->data = NULL;
    node->sons = old->sons;                     old->sons = Vector_Init(NULL);              // NULL returned not checked
    node->returns = old->returns;               old->returns = Vector_Init((void(*)(void*))Node_Destroy);   // NULL returned not checked
    node->semanticType = old->semanticType;     old->semanticType = SEMANTIC_BOOLEAN;
    node->count = old->count;                   old->count = 0;
    node->DataDtor = old->DataDtor;             old->DataDtor = NULL;
    node->operation = old->operation;           old->operation = P_EQ;

    if (old->sons == NULL || old->returns == NULL)
    {
        Node_Destroy(node, false);
        free(old);  // not correct but shit happens
        return false;
    }

    Node_AppendSon(old, node);
    Node_AppendSon(old, Node_Init(NODE_NIL, NULL, SEMANTIC_NIL, (void(*)(void*))NULL, P_VOID));
}

void Node_Destroy(Node *node, bool destroy)
{
    if (destroy)
        for (int i = 0; i < Vector_Size(node->sons); i++)
            Node_Destroy((Node*)Vector_GetElement(node->sons, i), true);


    Vector_Destroy(node->returns);
    Vector_Destroy(node->sons);
    free(node);
}

bool Node_AppendSon(Node *node, Node *son)
{
    return Vector_PushBack(node->sons, son);
}

bool Node_AppendReturn(Node *parent, SemanticType semanticType)
{
    Node *new_return = Node_Init(NODE_POP, NULL, semanticType, (void(*)(void*))NULL, P_VOID);
    if (new_return == NULL)
        return false;
    return Vector_PushBack(parent->returns, new_return);
}

void *Node_GetData(Node *node)
{
    return node->data;
}

void Node_SetParamCount(Node *node, int count)
{
    node->count = count;
}

int Node_GetParamCount(Node *node)
{
    return node->count;
}

Vector *Node_GetSons(Node *parent)
{
    return parent->sons;
}

Vector *Node_GetReturns(Node *parent)
{
    return parent->returns;
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

Vector* Node_PostOrder(Node *node, bool destroy, int offset, FILE *output)
{
    Vector *sons = Vector_Init(DataDtor);
    if (sons == NULL)
        return NULL;
    
    for (int i = 0; i < Vector_Size(node->sons); i++)
    {
        // should increase offset
        Vector *returned_values = Node_PostOrder((Node*)Vector_GetElement(node->sons, i), destroy, offset, output);
        if (returned_values == NULL)
            break;
        
        for (int j = 0; j < Vector_Size(returned_values); j++)
            Vector_PushBack(sons, Vector_GetElement(returned_values, j));

        // increase offset for net declared variable
        if (!Vector_IsEmpty(returned_values))
            offset = *((int*)Vector_Back(returned_values)) + 1;

        Vector_Destroy(returned_values);
    }

    Vector *return_values = Vector_Init(DataDtor);
    if (return_values == NULL)
        return NULL;

    switch (Node_GetType(node))
    {
    case NODE_OPERATION:
        break;

    case NODE_ID:
        fprintf(output, "MOVE %s%d %s%d\n", TMP, offset++, Element_GetKey(Node_GetData(node)), Element_GetID(Node_GetData(node)));
        break;

    case NODE_FUNCTION:
        break;

    case NODE_VALUE:
        // fprintf(output, "hello there\n");
        break;

    case NODE_POP:
        break;
    
    case NODE_NIL:
        break;

    default:
        break;
    }

    if (destroy)
        Node_Destroy(node, false);
    
    return return_values;
}