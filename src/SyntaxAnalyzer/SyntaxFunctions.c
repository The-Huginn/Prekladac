/**
 * @file SyntaxFunctions.c
 * @author Rastislav Budinsky
 * @brief This file implements SyntaxFunctions interface for SyntaxAnalyzer and code generation
 */
#include "SyntaxFunctions.h"
#include "SemanticActions.h"

#include <stdlib.h>

SemanticType Syntax_GetSemantic(Token *token)
{
    switch (Token_getType(token))
    {
    case K_BOOLEAN:
        return SEMANTIC_BOOLEAN;
        break;
    case K_INTEGER:
        return SEMANTIC_INTEGER;
        break;
    case K_NUMBER:
        return SEMANTIC_NUMBER;
        break;
    case K_STRING:
        return SEMANTIC_STRING;
        break;
    
    default:
        break;
    }
    return SEMANTIC_VOID;
}

bool Syntax_IsDatatype(Token *token)
{
    return Token_getType(token) == K_BOOLEAN || Token_getType(token) == K_INTEGER || Token_getType(token) == K_NUMBER || Token_getType(token) == K_STRING;
}

int Syntax_AddToTS(Symtable *symtable, Buffers *buffer, Token *token, SymbolType type)
{
    Element *previous = Symtable_GetElement(symtable, (const char*)Token_getData(token));

    Element *new_element = Symtable_CreateElement(symtable, (const char*)Token_getData(token), type);

    if (new_element == NULL)
        return 99;

    if (previous != NULL)
        if (Element_GetID(previous) == Element_GetID(new_element))
            return 3;

    Vector_PushBack(buffer->variables, new_element);
    buffer->position++;

    return -1;
}

int Syntax_Variable_SetSemantic(Buffers *buffer, Token *token)
{
    // more Semantic types then variables
    if (Vector_Size(buffer->variables) <= buffer->position)
        return 2;

    Element_SetSemantic((Element*)Vector_GetElement(buffer->variables, (buffer->position)), Syntax_GetSemantic(token));
    Element_Define((Element*)Vector_GetElement(buffer->variables, (buffer->position)++));
    return -1;
}

int Syntax_Variable_Assign(Buffers *buffer)
{
    // we should get rid of extra expressions
    while (Vector_Size(buffer->expressions) > Vector_Size(buffer->variables))
        Vector_PopBack(buffer->expressions);

    // we should fill with nils if we dont have enough
    // first we must check if last parameter is not an function
    int function_returns = 0;
    if (Vector_Size(buffer->expressions) > 0)
        if (Node_GetType((Node*)Vector_Back(buffer->expressions)) == NODE_FUNCTION)
            function_returns = Element_FunctionReturns_Size((Element*)Node_GetData((Node*)Vector_Back(buffer->expressions))) - 1;

    // so we dont skip unintentionally function returning nothing and then dont evaluate as correct assignment
    function_returns = function_returns < 0 ? 0 : function_returns;

    // if last function returns more expressions than needed
    function_returns = function_returns + Vector_Size(buffer->expressions) > Vector_Size(buffer->variables) ? Vector_Size(buffer->variables) - Vector_Size(buffer->expressions) : function_returns;

    if (Vector_Size(buffer->expressions) + function_returns < Vector_Size(buffer->variables))
        return 7;

    while (!Vector_IsEmpty(buffer->expressions))
    {
        if (function_returns != 0)
        {
            int current_variable = Vector_Size(buffer->variables) - 1;
            // TODO generate code for function call
            while (function_returns >= 0)
            {
                SemanticType type = Element_FunctionReturn_GetSemantic((Element*)Node_GetData((Node*)Vector_Back(buffer->expressions)), function_returns);
                if (Element_GetSemantic((Element*)Vector_GetElement(buffer->variables, current_variable)) != type)
                {
                    if (Element_GetSemantic((Element*)Vector_GetElement(buffer->variables, current_variable)) == SEMANTIC_BOOLEAN)
                        // compare with nil for boolean value with specific return value
                        AbstractSemanticTree_CompareWithNil((Node*)Vector_GetElement(Node_GetReturns((Node*)Vector_Back(buffer->expressions)), function_returns));
                    else
                        return 4;
                }

                function_returns--;
                current_variable--;

                Vector_PopBack(buffer->variables);
            }
            Vector_PopBack(buffer->expressions);

            function_returns = 0;
            // TODO generate code
        }
        else
        {
            SemanticType type = Node_GetSemantic((Node*)Vector_Back(buffer->expressions));
            if (Element_GetSemantic((Element*)Vector_Back(buffer->variables)) != type)
            {
                if (Element_GetSemantic((Element*)Vector_Back(buffer->variables)) == SEMANTIC_BOOLEAN)
                    // compare with nil for boolean value
                    AbstractSemanticTree_CompareWithNil((Node*)Vector_Back(buffer->expressions));
                else
                    return 4;
            }

            // TODO generate code
            Vector_PopBack(buffer->variables);
            Vector_PopBack(buffer->expressions);
        }
    }
    return -1;
}

int Syntax_Return_Assign(Buffers *buffer)
{
    // we should get rid of extra expressions
    while (Vector_Size(buffer->expressions) > Element_FunctionReturns_Size(buffer->current_function))
        Vector_PopBack(buffer->expressions);

    // we should fill with nils if we dont have enough
    // first we must check if last parameter is not an function
    int function_returns = 0;
    if (Vector_Size(buffer->expressions) > 0)
        if (Node_GetType((Node*)Vector_Back(buffer->expressions)) == NODE_FUNCTION)
            function_returns = Element_FunctionReturns_Size((Element*)Node_GetData((Node*)Vector_Back(buffer->expressions))) - 1;

    // so we dont skip unintentionally function returning nothing and then dont evaluate as correct assignment
    function_returns = function_returns < 0 ? 0 : function_returns;

    // if last function returns more expressions than needed
    function_returns = function_returns + Vector_Size(buffer->expressions) > Element_FunctionReturns_Size(buffer->current_function)
                            ? Element_FunctionReturns_Size(buffer->current_function) - Vector_Size(buffer->expressions)
                            : function_returns;

    // we fill last with nils
    int current_variable = Element_FunctionReturns_Size(buffer->current_function) - 1;

    if (Vector_Size(buffer->expressions) + function_returns < current_variable)
        return 7;

    while (!Vector_IsEmpty(buffer->expressions))
    {
        if (function_returns != 0)
        {
            // TODO generate code for function call
            while (function_returns >= 0)
            {
                SemanticType type = Element_FunctionReturn_GetSemantic((Element*)Node_GetData((Node*)Vector_Back(buffer->expressions)), function_returns);
                if (Element_FunctionReturn_GetSemantic(buffer->current_function, current_variable) != type)
                {
                    if (Element_FunctionReturn_GetSemantic(buffer->current_function, current_variable) == SEMANTIC_BOOLEAN)
                        // compare with nil for boolean value with specific return value
                        AbstractSemanticTree_CompareWithNil((Node*)Vector_GetElement((Vector*)Node_GetReturns((Node*)Vector_Back(buffer->expressions)), function_returns));
                    else
                        return 5;
                }

                function_returns--;
                current_variable--;
            }
            Vector_PopBack(buffer->expressions);

            function_returns = 0;
            // TODO generate code
        }
        else
        {
            SemanticType type = Node_GetSemantic((Node*)Vector_Back(buffer->expressions));
            if (Element_FunctionReturn_GetSemantic(buffer->current_function, current_variable) != type)
            {
                if (Element_FunctionReturn_GetSemantic(buffer->current_function, current_variable) == SEMANTIC_BOOLEAN)
                    // compare with nil for boolean value
                    AbstractSemanticTree_CompareWithNil((Node*)Vector_Back(buffer->expressions));
                else
                    return 5;
            }

            // TODO generate code
            current_variable--;
            Vector_PopBack(buffer->expressions);
        }
    }
    return -1;
}

int Syntax_FunctionCall(Symtable *symtable, Buffers *buffer)
{
    Element *function = Symtable_GetElement(symtable, Element_GetKey((Element*)Vector_Back(buffer->variables)));

    // calling undefined function
    if (function == NULL)
        return 3;

    Node *function_call = Node_Init(NODE_FUNCTION, Vector_Back(buffer->variables), SEMANTIC_VOID, NULL, P_FUNCTION);

    // create a function call node with all expressions as parameters, they are normalized no need for AST_UpdateFunctionCalls
    while (!Vector_IsEmpty(buffer->expressions))
    {
        Node_AppendSon(function_call, (Node*)Vector_GetElement(buffer->expressions, 0));
        Vector_RemoveElement(buffer->expressions, 0);
    }

    return AbstractSemanticTree_VerifyFunctionCall(function_call);
}
