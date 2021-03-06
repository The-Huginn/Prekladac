// IFJ Project 2021
/**
 * @file SyntaxFunctions.c
 * @author Rastislav Budinsky (xbudin05)
 * @brief This file implements SyntaxFunctions interface for SyntaxAnalyzer and code generation
 */
#include "SyntaxFunctions.h"
#include "SemanticActions.h"
#include "GenerateASS.h"

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
    {
        Node_Destroy((Node*)Vector_Back(buffer->expressions), true);
        Vector_PopBack(buffer->expressions);
    }

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

    int current_variable = Vector_Size(buffer->variables) - 1;
    for (int i = Vector_Size(buffer->expressions) - 1; i >= 0; i--)
    {
        if (function_returns != 0)
        {
            while (function_returns >= 0)
            {
                SemanticType type = Element_FunctionReturn_GetSemantic((Element*)Node_GetData((Node*)Vector_GetElement(buffer->expressions, i)), function_returns);
                if (Element_GetSemantic((Element*)Vector_GetElement(buffer->variables, current_variable)) != type)
                {
                    if (Element_GetSemantic((Element*)Vector_GetElement(buffer->variables, current_variable)) == SEMANTIC_BOOLEAN)
                        // compare with nil for boolean value with specific return value
                        AbstractSemanticTree_CompareWithNil((Node*)Vector_GetElement(Node_GetReturns((Node*)Vector_GetElement(buffer->expressions, i)), function_returns));
                    else
                        return 4;
                }

                function_returns--;
                current_variable--;
            }

            function_returns = 0;
        }
        else
        {
            SemanticType type = Node_GetSemantic((Node*)Vector_GetElement(buffer->expressions, i));
            if (Element_GetSemantic((Element*)Vector_GetElement(buffer->variables, current_variable)) != type)
            {
                if (Element_GetSemantic((Element*)Vector_GetElement(buffer->variables, current_variable)) == SEMANTIC_BOOLEAN)
                    // compare with nil for boolean value
                    AbstractSemanticTree_CompareWithNil((Node*)Vector_GetElement(buffer->expressions, i));
                else
                    return 4;
            }

            current_variable--;
        }
    }

    ASS_GenerateAssign(buffer);

    return -1;
}

int Syntax_Return_Assign(Buffers *buffer)
{
    // we should get rid of extra expressions
    while (Vector_Size(buffer->expressions) > Element_FunctionReturns_Size(buffer->current_function))
    {
        Node_Destroy((Node*)Vector_Back(buffer->expressions), true);
        Vector_PopBack(buffer->expressions);
    }

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

    int current_variable = Element_FunctionReturns_Size(buffer->current_function) - 1;

    if (Vector_Size(buffer->expressions) + function_returns <= current_variable)
        return 5;

    for (int i = Vector_Size(buffer->expressions) - 1; i >= 0; i--)
    {
        if (function_returns != 0)
        {
            // TODO generate code for function call
            while (function_returns >= 0)
            {
                SemanticType type = Element_FunctionReturn_GetSemantic((Element*)Node_GetData((Node*)Vector_GetElement(buffer->expressions, i)), function_returns);
                if (Element_FunctionReturn_GetSemantic(buffer->current_function, current_variable) != type)
                {
                    if (Element_FunctionReturn_GetSemantic(buffer->current_function, current_variable) == SEMANTIC_BOOLEAN)
                        // compare with nil for boolean value with specific return value
                        AbstractSemanticTree_CompareWithNil((Node*)Vector_GetElement((Vector*)Node_GetReturns((Node*)Vector_GetElement(buffer->expressions, i)), function_returns));
                    else
                        return 5;
                }

                function_returns--;
                current_variable--;
            }

            function_returns = 0;
        }
        else
        {
            SemanticType type = Node_GetSemantic((Node*)Vector_GetElement(buffer->expressions, i));
            if (Element_FunctionReturn_GetSemantic(buffer->current_function, current_variable) != type)
            {
                if (Element_FunctionReturn_GetSemantic(buffer->current_function, current_variable) == SEMANTIC_BOOLEAN)
                    // compare with nil for boolean value
                    AbstractSemanticTree_CompareWithNil((Node*)Vector_GetElement(buffer->expressions, i));
                else
                    return 5;
            }

            current_variable--;
        }
    }

    ASS_GenerateFunctionReturn(buffer);

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
    for (int i = 0; i < Vector_Size(buffer->expressions); i++)
        Node_AppendSon(function_call, (Node*)Vector_GetElement(buffer->expressions, i));

    int ret = AbstractSemanticTree_VerifyFunctionCall(function_call);
    if (ret != -1)
        return ret;

    ASS_GenerateFunctionCall(buffer, function_call);

    return -1;
}

int Syntax_FunctionDefined(Buffers *buffer, Symtable *symtable)
{
    Symtable_AddScope(symtable);

    // clear variables
    Vector_Clear(buffer->variables);

    int return_value = -1;
    // add all parameters from current function to TS
    for (int i = 0; i < Element_FunctionParameters_Size(buffer->current_function); i++)
    {
        Element *previous_parameter = Symtable_GetElement(symtable, Element_FunctionParameter_GetName(buffer->current_function, i));
        Element* parameter = Symtable_CreateElement(symtable, Element_FunctionParameter_GetName(buffer->current_function, i),VARIABLE);
        if (parameter == NULL)
        {
            return_value = 99;
            break;
        }

        // parameters with same name
        if (previous_parameter != NULL)
            if (Element_GetID(parameter) == Element_GetID(previous_parameter))
                return_value = 3;
                    
        Element_SetSemantic(parameter, Element_FunctionParameter_GetSemantic(buffer->current_function, i));
        Element_Define(parameter);

        // for code generation
        Vector_PushBack(buffer->variables, parameter);
    }

    // generate code for function label
    if (return_value == -1)
        ASS_AddFunction(buffer);

    return return_value;
}
