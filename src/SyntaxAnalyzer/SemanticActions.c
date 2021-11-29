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
#include <string.h>

int AbstractSemanticTree_CompareWithNil(Node *node)
{
    if (Node_GetType(node) == NODE_FUNCTION)
    {
        Vector *returns = Node_GetReturns(node);

        // no returns from function
        if (Vector_Size(returns) == 0)
            return 5;
            
        if (Node_CompareWithNil(Vector_GetElement(returns, 0)) == false)
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
    if (Node_GetData(root) == NULL)
    {
        WARNING("No data yet");
        return 2;
    }

    // Exception with write() function, which has variadic amount of parameters
    if (strcmp(Element_GetKey(Node_GetData(root)), "write\0") == 0)
        return -1;
        
    // function was called with too many parameters
    if (Element_FunctionParameters_Size(Node_GetData(root)) < Vector_Size(Node_GetSons(root)))
        return 5;

    // we dont have same count of parameters, re-adjustments needed
    if (Element_FunctionParameters_Size(Node_GetData(root)) > Vector_Size(Node_GetSons(root)))
    {
        Node *last_param = Vector_Back(Node_GetSons(root));

        if (last_param == NULL)
            return 5;

        // last is not function so params missing
        if (Node_GetType(last_param) != FUNCTION)
            return 5;


        Element *last_function = Node_GetData(last_param);

        // last function has less returns than needed
        if (Element_FunctionParameters_Size(Node_GetData(root)) > (Vector_Size(Node_GetSons(root)) - 1) + Element_FunctionReturns_Size(last_function))
            return 5;

        // we need this many extra returns from the function
        Node_SetParamCount(root, Element_FunctionParameters_Size(Node_GetData(root)) - Vector_Size(Node_GetSons(root)));
    }

    // we check all available params once
    for (int i = 0; i < Vector_Size(Node_GetSons(root)); i++)
        if (Node_GetSemantic(Vector_GetElement(Node_GetSons(root), i)) != Element_FunctionParameter_GetSemantic(Node_GetData(root), i))
        {
            // if boolean as parameter we should convert to be compared with nil
            if (Element_FunctionParameter_GetSemantic(Node_GetData(root), i) == SEMANTIC_BOOLEAN)
            {
                int ret = AbstractSemanticTree_CompareWithNil(Vector_GetElement(Node_GetSons(root), i));
                if (ret != -1)
                    return ret;
            }
            else
                return 5;
        }

    // if we have any parameters we might check
    if (!Vector_IsEmpty(Node_GetSons(root)))
    {
        // only if last parameter is function call
        if (Node_GetType(Vector_Back(Node_GetSons(root))) == FUNCTION)
        {
            // vector holding POP-ing expressions of function call
            Vector *last_function = Node_GetReturns(Vector_Back(Node_GetSons(root)));
            int size = Vector_Size(Node_GetSons(root));

            for (int i = 0; i < Node_GetParamCount(root); i++)
                // last_function is Vector<Node*>
                if (Node_GetSemantic(Vector_GetElement(last_function, i + 1)) != Element_FunctionParameter_GetSemantic(Node_GetData(root), size + i + 1))
                {
                    if (Element_FunctionParameter_GetSemantic(Node_GetData(root), size + i + 1) == SEMANTIC_BOOLEAN)
                    {
                        int ret = AbstractSemanticTree_CompareWithNil(Vector_GetElement(last_function, i + 1));
                        if (ret != -1)
                            return ret;
                    }
                    else
                        return 5;
                }
        }
    }

    // returns something
    if (Element_FunctionReturns_Size(Node_GetData(root)) > 0)
        // changes semantic to first return value
        Node_SetSemantic(root, Element_FunctionReturn_GetSemantic(Node_GetData(root), 0));
    
    return -1;
}

bool AbstractSemanticTree_IsZero(Node *node)
{
    if (Node_GetType(node) != NODE_VALUE)
        return false;

    bool isZero = true;
    char *string = Node_GetData(node);

    while (*string != '\0' && isZero)
    {
        // we expect valid token so we dont consider 0.0.0 as a problem, should be detected by LexicalAnalyzer
        isZero = (*string) == '0' || (*string) == '.';
        string++;
    }

    return isZero;
}

/**
 * @note parameters are in reverse order
 */
int AbstractSemanticTree_BinaryOperator(Node *root)
{    
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
    {
        if (first != SEMANTIC_NIL && second != SEMANTIC_NIL)
            return 6;
        // one of them at least is nil, we can only EQ / NEQ with nil
        if (Node_GetOperation(root) != P_EQ && Node_GetOperation(root) != P_NEQ)
            return 8;
    }

    switch (Node_GetOperation(root))
    {
    case P_DIV:
        if (AbstractSemanticTree_IsZero(Vector_GetElement(Node_GetSons(root), 0)) == true)
            return 9;

        if (first != SEMANTIC_INTEGER && first != SEMANTIC_NUMBER)
            return 6;
        first = SEMANTIC_NUMBER;
        break;
    case P_MUL:
    case P_PLUS:
    case P_MINUS:
        if (first != SEMANTIC_INTEGER && first != SEMANTIC_NUMBER)
            return 6;
        break;
    
    case P_INT_DIV:
        if (AbstractSemanticTree_IsZero(Vector_GetElement(Node_GetSons(root), 0)) == true)
            return 9;

        if (first != SEMANTIC_INTEGER)
            return 6;
        break;

    // we can concat with nil probably
    case P_CONCAT:
        if (first != SEMANTIC_STRING)
            if (first != SEMANTIC_NIL && second != SEMANTIC_NIL)
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
        if (first != SEMANTIC_INTEGER && first != SEMANTIC_NUMBER && first != SEMANTIC_BOOLEAN && first != SEMANTIC_NIL && first != SEMANTIC_STRING)
            return 6;
        break;

    case P_AND:
    case P_OR:
        if (first != SEMANTIC_BOOLEAN)
        {
            int ret = AbstractSemanticTree_CompareWithNil(Vector_GetElement(Node_GetSons(root), 0));
            if (ret != -1)
                return ret;
            first = SEMANTIC_BOOLEAN;
        }
        if (second != SEMANTIC_BOOLEAN)
        {
            int ret = AbstractSemanticTree_CompareWithNil(Vector_GetElement(Node_GetSons(root), 1));
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
    
    if (first == SEMANTIC_NIL)
        return 8;

    switch (Node_GetOperation(root))
    {
    case P_LEN:
        if (first != SEMANTIC_STRING)
            return 6;
        first = SEMANTIC_INTEGER;
        break;
    
    case P_NOT:
        if (first != SEMANTIC_BOOLEAN)
        {
            int ret = AbstractSemanticTree_CompareWithNil(Vector_GetElement(Node_GetSons(root), 0));
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