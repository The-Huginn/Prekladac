/**
 * @file node.c
 * @author Rastislav Budinsky
 * @brief This file implements struct Node and it's interface
 */
#include "node.h"
#include "logger.h"
#include "symbolelement.h"

#include <stdlib.h>
#include <string.h>

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

    node->returns = Vector_Init((void(*)(void*))NULL);
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

    return true;
}

void Node_Destroy(Node *node, bool destroy)
{
    if (destroy)
    {
        for (int i = 0; i < Vector_Size(node->sons); i++)
            Node_Destroy((Node*)Vector_GetElement(node->sons, i), true);
        for (int i = 0; i < Vector_Size(node->returns); i++)
            Node_Destroy((Node*)Vector_GetElement(node->returns, i), true);
    }


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

void Number_Destroy(void *data)
{
    free(data);
}

void *Number_Init(int number)
{
    void *createdNumber = (void*)malloc(sizeof(int));
    if (createdNumber == NULL)
        return NULL;

    *((int*)createdNumber) = number;
    
    return createdNumber;
}

bool Node_IsBinaryOperator(PrecedenceItemType type)
{
    return
        (type == P_MUL ||
        type == P_DIV ||
        type == P_INT_DIV ||
        type == P_PLUS ||
        type == P_MINUS ||
        type == P_CONCAT ||
        type == P_GRT ||
        type == P_LESS ||
        type == P_GEQ ||
        type == P_LEQ ||
        type == P_EQ ||
        type == P_NEQ ||
        type == P_AND ||
        type == P_OR)

        ? true : false;
}

Vector* Node_PostOrder(Node *node, bool destroy, Buffers *buffer, int expected_amount)
{
    Vector *sons = Vector_Init(Number_Destroy);
    if (sons == NULL)
        return NULL;
    
    for (int i = 0; i < Vector_Size(Node_GetSons(node)); i++)
    {
        Vector *returned_values = NULL;

        // This condition is probably unnecessary and we can just use the first expression as Node_GetParamCount is by default 0
        // we need from last function as parameter maybe more returns
        if (Node_GetType(node) == FUNCTION && i == Vector_Size(Node_GetSons(node)) - 1)
            // 1 + Node_GetParamCount(node) as we want at least always one parameter and maybe more
            returned_values = Node_PostOrder((Node*)Vector_GetElement(Node_GetSons(node), i), destroy, buffer, 1 + Node_GetParamCount(node));
        else
            returned_values = Node_PostOrder((Node*)Vector_GetElement(Node_GetSons(node), i), destroy, buffer, 1);
        if (returned_values == NULL)
            break;
        
        for (int j = 0; j < Vector_Size(returned_values); j++)
            Vector_PushBack(sons, Number_Init(*((int*)Vector_GetElement(returned_values, j))));

        // increase offset for net declared variable
        if (!Vector_IsEmpty(returned_values))
            buffer->tmp_offset = *((int*)Vector_Back(returned_values)) + 1;

        Vector_Destroy(returned_values);
    }

    Vector *return_values = Vector_Init(Number_Destroy);
    if (return_values == NULL)
        return NULL;

    switch (Node_GetType(node))
    {
    case NODE_OPERATION:
        if (Node_IsBinaryOperator(Node_GetOperation(node)))
        {
            bool compare_with_not = false;
            switch (Node_GetOperation(node))
            {
            case P_MUL:
                fprintf(buffer->output, "MUL");
                break;
            
            case P_DIV:
                fprintf(buffer->output, "DIV");
                break;
            
            case P_INT_DIV:
                fprintf(buffer->output, "IDIV");
                break;

            case P_PLUS:
                fprintf(buffer->output, "ADD");
                break;

            case P_MINUS:
                fprintf(buffer->output, "SUB");
                break;

            case P_CONCAT:
                fprintf(buffer->output, "CONCAT");
                break;

            case P_GRT:
                fprintf(buffer->output, "GT");
                break;
            
            case P_LESS:
                fprintf(buffer->output, "LT");
                break;

            case P_GEQ: // we need to compare it LT and then NOT
                fprintf(buffer->output, "LT");
                compare_with_not = true;
                break;

            case P_LEQ: // we need to compare with GT and then NOT
                fprintf(buffer->output, "GT");
                compare_with_not = true;
                break;

            case P_EQ:
                fprintf(buffer->output, "EQ");
                break;

            case P_NEQ: // we need to compare with EQ and then NOT
                fprintf(buffer->output, "EQ");
                compare_with_not = true;
                break;

            case P_AND:
                fprintf(buffer->output, "AND");
                break;

            case P_OR:
                fprintf(buffer->output, "OR");
                break;
            
            default:
                break;
            }

            fprintf(buffer->output, " LF@%s%d LF@%s%d LF@%s%d\n", TMP(buffer->tmp_offset), TMP(*((int*)Vector_GetElement(sons, 0))), TMP(*((int*)Vector_GetElement(sons, 1))));

            if (compare_with_not)
            {
                buffer->tmp_offset++;
                fprintf(buffer->output, "NOT LF@%s%d LF@%s%d\n", TMP(buffer->tmp_offset), TMP(buffer->tmp_offset - 1));
            }

            Vector_PushBack(return_values, Number_Init(buffer->tmp_offset));
        }
        else    // Unary
        {
            switch (Node_GetOperation(node))
            {

            case P_LEN:
                fprintf(buffer->output, "STRLEN");
                break;

            case P_NOT:
                fprintf(buffer->output, "NOT");
                break;

            default:
                break;
            }

            fprintf(buffer->output, " LF@%s%d LF@%s%d\n", TMP(buffer->tmp_offset), TMP(*((int*)Vector_GetElement(sons, 0))));
            Vector_PushBack(return_values, Number_Init(buffer->tmp_offset));
        }
        break;

    case NODE_ID:
        fprintf(buffer->output, "MOVE LF@%s%d LF@%s%d\n", TMP(buffer->tmp_offset), ELEMENT(Node_GetData(node)));
        Vector_PushBack(return_values, Number_Init(buffer->tmp_offset));
        break;

    case NODE_VALUE:
        fprintf(buffer->output, "MOVE LF@%s%d ", TMP(buffer->tmp_offset));
        switch (Node_GetSemantic(node))
        {
        case SEMANTIC_BOOLEAN:
            fprintf(buffer->output, "bool");
            break;

        case SEMANTIC_INTEGER:
            fprintf(buffer->output, "int");
            break;
            
        case SEMANTIC_STRING:
            fprintf(buffer->output, "string");
            break;
        
        case SEMANTIC_NUMBER:
            fprintf(buffer->output, "number");
            break;

        default:
            WARNING("other NODE_VALUE semantic type");
            break;
        }

        fprintf(buffer->output, "@%s\n", (const char*)Node_GetData(node));
        Vector_PushBack(return_values, Number_Init(buffer->tmp_offset));

        break;

    case NODE_FUNCTION:
        for (int i = Vector_Size(sons) - 1; i >= 0; i--)
            fprintf(buffer->output, "PUSHS LF@%s%d\n", TMP(*((int*)Vector_GetElement(sons, i))));

        // because of convention we have decided to push number of variadic parameters in function write
        if (strcmp(Element_GetKey((Element*)Node_GetData(node)), "write\0") == 0)
            fprintf(buffer->output, "PUSHS int@%d\n", Vector_Size(sons));

        fprintf(buffer->output, "CALL LF@%s%d\n", Element_GetKey((Element*)Node_GetData(node)), Element_GetID((Element*)Node_GetData(node)));

        Vector *returns = Vector_Init(Number_Destroy);
        if (returns == NULL)
            return NULL;

        // get all returns from function in returns Vector
        for (int i = 0; i < Vector_Size(Node_GetReturns(node)); i++)
        {
            Vector *returned_values = Node_PostOrder((Node*)Vector_GetElement(Node_GetReturns(node), i), destroy, buffer, 1);
            if (returned_values == NULL)
                break;
            
            for (int j = 0; j < Vector_Size(returned_values); j++)
                Vector_PushBack(returns, Number_Init(*((int*)Vector_GetElement(returned_values, j))));

            // increase offset for net declared variable
            if (!Vector_IsEmpty(returned_values))
                buffer->tmp_offset = *((int*)Vector_Back(returned_values)) + 1;

            Vector_Destroy(returned_values);
        }

        // only first few parameters return
        for (int i = 0; i < expected_amount; i++)
            Vector_PushBack(return_values, Number_Init(*((int*)Vector_GetElement(returns, i))));
        
        Vector_Destroy(returns);

        break;

    case NODE_POP:
        fprintf(buffer->output, "POPS LF@%s%d\n", TMP(buffer->tmp_offset));
        Vector_PushBack(return_values, Number_Init(buffer->tmp_offset));
        break;
    
    case NODE_NIL:
        fprintf(buffer->output, "MOVE %s%d nil@nil\n", TMP(buffer->tmp_offset));
        Vector_PushBack(return_values, Number_Init(buffer->tmp_offset));
        break;

    default:
        break;
    }

    Vector_Destroy(sons);

    if (destroy)
        Node_Destroy(node, false);
    
    return return_values;
}