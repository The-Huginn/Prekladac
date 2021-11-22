/**
 * @file SemanticActions.c
 * @author Rastislav Budinsky
 * @brief Contains implementation of semantic functions and supportive functions 
 */
#include "SemanticActions.h"
#include "PrecedenceTable.h"
#include "../Utils/symbolelement.h"
#include "../Utils/logger.h"

#include <stdlib.h>

#define NOT_IMPLEMENTED

/**
 * @brief Adds new node where it compares with nil, if we expect function index can be added
 * @param node Node which needs to be compared with nil
 * @param index Set default to 0 unless in function call as function return
 * @return -1 on success otherwise error code
 */
int AbstractSemanticTree_CompareWithNil(Node *node, int index)
{
    if (Node_GetType(node) == NODE_FUNCTION)
    {
        Vector *returns = Node_GetReturns(node);

        // no returns from function
        if (Vector_Size(returns) == 0)
            return 5;
            
        if (Node_CompareWithNil(Vector_GetElement(returns, index)) == false)
            return 99;
    }
    else
    {
        if (Node_CompareWithNil(node) == false)
            return 99;
    }

    return -1;
}

//              E
//            / , \
//          E      \
//        / , \     \
//       E     E     \
//     / , \    \     \
//    E    E    E     E
// Tree of arguments should look like this
// keep in mind this tree is mirrored as the rules are applied from the end. (Probably, works like this looks like)
bool AbstractSemanticTree_UpdateFunctionCalls(Node *root)
{
    Vector *new_sons = Vector_Init(NULL);
    if (new_sons == NULL)
        return false;

    Vector *children = Node_GetSons(root);
    Node *child = (Node*)Vector_GetElement(children, 0);

    while (Node_IsOperation(child) && Node_GetOperation(child) == P_COMMA)
    {
        children = Node_GetSons(child);

        // See function comments
        // This is why indexes 0 and 1 are switch compared to the picture above
        Vector_PushBack(new_sons, Vector_GetElement(children, 0));
        
        Node *next = (Node*)Vector_GetElement(children, 1);

        Node_Destroy(child, false);
        child = next;
    }

    Vector_PushBack(new_sons, child);

    // We want to ignore Void parameter
    Vector_PopBack(new_sons);

    // we clear current parameters
    Vector_Clear(Node_GetSons(root));


    // append new parameters in correct order
    while (!Vector_IsEmpty(new_sons))
    {
        if (Node_AppendSon(root, Vector_Back((void*)new_sons)))
            Vector_PopBack(new_sons);
        else
        {
            Vector_Destroy(new_sons);
            return false;
        }
    }

    return true;
}

int AbstractSemanticTree_VerifyFunctionCall(Node *root)
{
    #ifdef NOT_IMPLEMENTED
    return -1;
    #endif

    if (Node_GetData(root) == NULL)
    {
        WARNING("No data yet");
        return 2;
    }
        
    // function was called with too many parameters
    if (Vector_Size(Element_GetFunctionParameters(Node_GetData(root))) < Vector_Size(Node_GetSons(root)))
        return 5;

    // we dont have same count of parameters, re-adjustments needed
    if (Vector_Size(Element_GetFunctionParameters(Node_GetData(root))) > Vector_Size(Node_GetSons(root)))
    {
        // at least one param is here
        Node *last_param = Vector_Back(Node_GetSons(root));

        // last is not function so params missing
        if (Node_GetType(last_param) != FUNCTION)
            return 5;


        Element *last_function = Node_GetData(last_param);

        // last function has less returns than needed
        if (Vector_Size(Element_GetFunctionParameters(Node_GetData(root))) >
            (Vector_Size(Node_GetSons(root)) - 1) + Vector_Size(Element_GetFunctionReturns(last_function)))
            return 5;

        // we need this many extra returns from the function
        Node_SetParamCount(root, Vector_Size(Element_GetFunctionParameters(Node_GetData(root))) - Vector_Size(Node_GetSons(root)));
    }

    Vector *function_call = Element_GetFunctionParameters(Node_GetData(root));

    // we check all available params once
    for (int i = 0; i < Vector_Size(Node_GetSons(root)); i++)
        if (Node_GetSemantic(Vector_GetElement(Node_GetSons(root), i)) != *((SemanticType*)Vector_GetElement(function_call, i)) &&
            *((SemanticType*)Vector_GetElement(function_call, i)) != SEMANTIC_BOOLEAN)
            return 5;

    Vector *last_function = Element_GetFunctionReturns(Node_GetData(Vector_Back(Node_GetSons(root))));
    int size = Vector_Size(Node_GetSons(root));

    for (int i = 0; i < Node_GetParamCount(root); i++)
        if (*((SemanticType*)Vector_GetElement(last_function, i + 1)) != *((SemanticType*)Vector_GetElement(function_call, size + i + 1)) &&
            *((SemanticType*)Vector_GetElement(function_call, size + i + 1)) != SEMANTIC_BOOLEAN)
            return 5;

    // returns something
    if (Vector_Size(Element_GetFunctionReturns(Node_GetData(root))) > 0)
        // changes semantic to first return value
        Node_SetSemantic(root, *((SemanticType*)Vector_GetElement(Element_GetFunctionReturns(Node_GetData(root)), 0)));
    
    return -1;
}

int AbstractSemanticTree_BinaryOperator(Node *root)
{
    #ifdef NOT_IMPLEMENTED
    return -1;
    #endif
    
    if (!Node_IsOperation(root))
    {
        WARNING("Expected Operator");
        return 7;
    }

    if (Vector_Size(Node_GetSons(root)) != 2)
    {
        WARNING("Binary operator called with no 2 operands");
        return 7;
    }

    SemanticType first = Node_GetSemantic(Vector_GetElement(Node_GetSons(root), 0));
    SemanticType second = Node_GetSemantic(Vector_GetElement(Node_GetSons(root), 1));

    if (first != second)
        return 6;

    switch (Node_GetOperation(root))
    {
    case P_MUL:
    case P_DIV:
    case P_PLUS:
    case P_MINUS:
        if (first != SEMANTIC_INTEGER && first != SEMANTIC_NUMBER)
            return 6;
        break;
    
    case P_INT_DIV:
        if (first != SEMANTIC_INTEGER)
            return 6;
        break;

    case P_CONCAT:
        if (first != SEMANTIC_STRING)
            return 6;
        break;

    case P_LESS:
    case P_GRT:
    case P_LEQ:
    case P_GEQ:
        if (first != SEMANTIC_INTEGER && first != SEMANTIC_NUMBER)
            return 6;
        break;

    case P_EQ:
    case P_NEQ:
        if (first != SEMANTIC_INTEGER && first != SEMANTIC_NUMBER && first != SEMANTIC_BOOLEAN)
        break;

    case P_AND:
    case P_OR:
        if (first != SEMANTIC_BOOLEAN)
        {
            int ret = AbstractSemanticTree_CompareWithNil(Vector_GetElement(Node_GetSons(root), 0), 0);
            if (ret != -1)
                return ret;
            first = SEMANTIC_BOOLEAN;
        }
        if (second != SEMANTIC_BOOLEAN)
        {
            int ret = AbstractSemanticTree_CompareWithNil(Vector_GetElement(Node_GetSons(root), 1), 0);
            if (ret != -1)
                return ret;
        }
        break;
    // not a binary operator
    default:
        return 7;
    }

    Node_SetSemantic(root, first);

    return -1;
}

int AbstractSemanticTree_UnaryOperator(Node *root)
{
    #ifdef NOT_IMPLEMENTED
    return -1;
    #endif
    
    if (!Node_IsOperation(root))
    {
        WARNING("Expected Operator");
        return 7;
    }

    if (Vector_Size(Node_GetSons(root)) != 1)
    {
        WARNING("Unary operator called with no 1 operand");
        return 7;
    }

    SemanticType first = Node_GetSemantic(Vector_GetElement(Node_GetSons(root), 0));

    switch (Node_GetOperation(root))
    {
    case P_LEN:
        if (first != SEMANTIC_STRING)
            return 6;
        break;
    
    case P_NOT:
        if (first != SEMANTIC_BOOLEAN)
        {
            int ret = AbstractSemanticTree_CompareWithNil(Vector_GetElement(Node_GetSons(root), 0), 0);
            if (ret != -1)
                return ret;
            first = SEMANTIC_BOOLEAN;
        }
        break;

    // not an unary operator
    default:
        return 7;
    }

    Node_SetSemantic(root, first);

    return -1;
}