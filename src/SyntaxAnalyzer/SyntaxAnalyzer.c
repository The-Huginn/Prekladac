/**
 * @file SyntaxAnalyzer.c
 * @brief This file implements interface of SyntaxAnalyzer.h, contains custom functions
 * @author Rastislav Budinsky
 */
#include "SyntaxAnalyzer.h"
#include "Terminal.h"
#include "NonTerminal.h"
#include "LLTable.h"
#include "BottomToTop.h"
#include "SemanticActions.h"

#include "../Utils/stack.h"
#include "../Utils/list.h"
#include "../Utils/vector.h"
#include "../Utils/symtable.h"

#include "../LexicalAnalyzer/LexicalAnalyzer.h"


#include <stdlib.h>

typedef enum {FSM_START, FSM_VAR_DEC, FSM_VAR_DATATYPES, FSM_VAR_ASSIGN, FSM_ID, FSM_FUN_CALL, FSM_FUN_PARAMS, FSM_FUN_DEF, FSM_FUN_DEF_PARAMS, FSM_FUN_DEF_WAIT, FSM_FUN_DEF_RETURNS, FSM_FUN_DEC, FSM_FUN_DEC_PARAMS, FSM_FUN_DEC_WAIT, FSM_FUN_DEC_RETURNS, FSM_RETURNS}FSM_STATE;

typedef struct
{
    Vector *variables;
    Vector *expressions;
    int position;
    bool declared;
    Element *current_function;
}Buffers;

// #define DEBUG_SYNTAX


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

int Syntax_SetSemantic(Buffers *buffer, Token *token)
{
    if (Vector_Size(buffer->variables) <= buffer->position)
        return 2;

    Element_SetSemantic((Element*)Vector_GetElement(buffer->variables, (buffer->position)), Syntax_GetSemantic(token));
    Element_Define((Element*)Vector_GetElement(buffer->variables, (buffer->position)++));
    return -1;
}

int Syntax_Assign(Buffers *buffer)
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

    while (Vector_Size(buffer->expressions) + function_returns < Vector_Size(buffer->variables))
    {
        // TODO generate nil assignment
        Vector_PopBack(buffer->variables);
    }

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

int Syntax_Return(Buffers *buffer)
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
    while (Vector_Size(buffer->expressions) + function_returns < current_variable)
    {
        // TODO generate nil assignment to return variable
        current_variable--;
    }

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

int Syntax_FSM_Action(FSM_STATE *state, Token *token, int *return_value, Symtable *symtable, Buffers *buffer)
{
    // state variable actions
    switch (*state)
    {
    case FSM_VAR_DEC:           // if we ID we add it to the TS otherwise we go to datatypes for our variables
        // :
        if (Token_getType(token) == T_DEF)
        {
            buffer->position = 0;
            *state = FSM_VAR_DATATYPES;
        }
        else if (Token_getType(token) == T_ID)
            *return_value = Syntax_AddToTS(symtable, buffer, token, VARIABLE);
        break;

    case FSM_VAR_DATATYPES:     // if we get '=' we go to assign, if we get datatype of ',' we stay and update variables otherwise START
        if (Token_getType(token) == T_ASS)
        {
            // we have not declared all variables properly
            if (buffer->position != Vector_Size(buffer->variables))
                *return_value = 2;
            *state = FSM_VAR_ASSIGN;
        }
        else if (Token_getType(token) == K_BOOLEAN || Token_getType(token) == K_INTEGER || Token_getType(token) == K_NUMBER || Token_getType(token) == K_STRING)
            *return_value = Syntax_SetSemantic(buffer, token);
        else if (Token_getType(token) != T_COMMA)
            // no need to clear vector, gets cleared when in state = FSM_START
            *state = FSM_START;
        break;

    case FSM_VAR_ASSIGN: // unless we get another terminal to pop other than ',' we assign expressions
        if (Token_getType(token) != T_COMMA)
        {
            *return_value = Syntax_Assign(buffer);
            *state = FSM_START;
        }
        break;

    case FSM_ID:
        if (Token_getType(token) == T_ID)
        {
            Element *element = Symtable_GetElement(symtable, (const char*)Token_getData(token));
            // undeclared
            if (element == NULL)
            {
                *return_value = 3;
                break;
            }
            // undefined
            if (!Element_IsDefined(element))
                *return_value = 3;
            Vector_PushBack(buffer->variables, element);
        }
        else if (Token_getType(token) == T_LEFT)
            *state = FSM_FUN_PARAMS;
        else if (Token_getType(token) == T_ASS)
            *state = FSM_VAR_ASSIGN;
        else if (Token_getType(token) != T_COMMA)
            *state = FSM_START;
        break;

    case FSM_FUN_CALL:      // if we get ID it's function's ID otherwise with '(' we move to expressions
        if (Token_getType(token) == T_ID)
        {
            Element *function = Symtable_GetElement(symtable, (const char*)Token_getData(token));
            if (function == NULL)
                *return_value = 3;
            else if (Element_IsDefined(function) == false)
                *return_value = 3;
            
            Vector_PushBack(buffer->variables, function);
        }
        else if (Token_getType(token) == T_LEFT)
            *state = FSM_FUN_PARAMS;
        break;
    
    case FSM_FUN_PARAMS:        // if we get ')' we end otherwise just add expressions as in FSM_VAR_ASSIGN
        if (Token_getType(token) == T_RIGHT)
        {
            *return_value = Syntax_FunctionCall(symtable, buffer);
            *state = FSM_START;
        }
        break;

    case FSM_FUN_DEC:
        if (Token_getType(token) == T_ID)
        {
            Element *function = Symtable_GetElement(symtable, (const  char*)Token_getData(token));
            if (function != NULL)   // redeclaration
            {
                *return_value = 3;
                break;
            }
            
            function = Symtable_CreateElement(symtable, (const char*)Token_getData(token), FUNCTION);
            if (function == NULL)
                *return_value = 99;
            Vector_PushBack(buffer->variables, function);
        }
        else if (Token_getType(token) == T_LEFT)
            *state = FSM_FUN_DEC_PARAMS;
        break;

    case FSM_FUN_DEC_PARAMS:
        if (Token_getType(token) == T_RIGHT)
            *state = FSM_FUN_DEC_WAIT;
        else if (Token_getType(token) == K_BOOLEAN || Token_getType(token) == K_INTEGER || Token_getType(token) == K_NUMBER || Token_getType(token) == K_STRING)
            Element_AddParam((Element*)Vector_Back(buffer->variables), Syntax_GetSemantic(token), NULL);
        break;

    case FSM_FUN_DEC_WAIT:
        if (Token_getType(token) == T_DEF)
            *state = FSM_FUN_DEC_RETURNS;
        else
            *state = FSM_START;
        break;

    case FSM_FUN_DEC_RETURNS:
        if (Token_getType(token) == K_BOOLEAN || Token_getType(token) == K_INTEGER || Token_getType(token) == K_NUMBER || Token_getType(token) == K_STRING)
            Element_AddReturn((Element*)Vector_Back(buffer->variables), Syntax_GetSemantic(token));
        else if (Token_getType(token) != T_COMMA)
            *state = FSM_START;
        break;

    case FSM_FUN_DEF:
        if (Token_getType(token) == T_ID)
        {
            Element *function = Symtable_GetElement(symtable, (const char*)Token_getData(token));
            if (function != NULL)   // is declared
            {
                buffer->declared = true;
                *return_value = Element_IsDefined(function) ? 3 : -1;    // redifinition
            }
            else
                function = Symtable_CreateElement(symtable, (const char*)Token_getData(token), FUNCTION);
            if (function == NULL)
            {
                *return_value = 99;
                break;
            }
            Vector_PushBack(buffer->variables, function);
            // define function
            Element_Define(function);
            // Function we are currently parsing, we need this to have semantic actions for return
            buffer->current_function = function;
        }
        else if (Token_getType(token) == T_LEFT)
            *state = FSM_FUN_DEF_PARAMS;
        break;

    case FSM_FUN_DEF_PARAMS:
        if (Token_getType(token) == T_RIGHT)
            *state = FSM_FUN_DEF_WAIT;
        else if (Token_getType(token) == T_ID)
        {
            if (buffer->declared == true)
                Element_FunctionParameter_SetName((Element*)Vector_Back(buffer->variables), buffer->position, (const char*)Token_getData(token));
            else
                Element_AddParam((Element*)Vector_Back(buffer->variables), SEMANTIC_VOID, (const char*)Token_getData(token));
        }
        else if (Token_getType(token) == K_BOOLEAN || Token_getType(token) == K_INTEGER || Token_getType(token) == K_NUMBER || Token_getType(token) == K_STRING)
        {
            if (buffer->declared == true)
                // semantic types must equal otherwise "redeclaration" of parameter
                *return_value = Syntax_GetSemantic(token) == Element_FunctionParameter_GetSemantic((Element*)Vector_Back(buffer->variables), (buffer->position)++) ? -1 : 3;
            else
                Element_FunctionParameter_SetSemantic((Element*)Vector_Back(buffer->variables), (buffer->position)++, Syntax_GetSemantic(token));
        }
        break;

    case FSM_FUN_DEF_WAIT:
        // reset position for possible returns
        buffer->position = 0;
        if (Token_getType(token) == T_DEF)
            *state = FSM_FUN_DEF_RETURNS;
        else
        {
            Symtable_AddScope(symtable);
            *state = FSM_START;
        }
        break;

    case FSM_FUN_DEF_RETURNS:
        if (Token_getType(token) == K_BOOLEAN || Token_getType(token) == K_INTEGER || Token_getType(token) == K_NUMBER || Token_getType(token) == K_STRING)
        {
            if (buffer->declared == true)
                *return_value = Syntax_GetSemantic(token) == Element_FunctionReturn_GetSemantic((Element*)Vector_Back(buffer->variables), (buffer->position)++) ? -1 : 3;
            else
                Element_AddReturn((Element*)Vector_Back(buffer->variables), Syntax_GetSemantic(token));
        }
        else if (Token_getType(token) != T_COMMA)
        {
            Symtable_AddScope(symtable);
            *state = FSM_START;
        }
        break;


    default:
        break;
    }
}

/**
 * @brief Adds built-in-functions
 * @param symtable Symtable
 * @return 0 on success
 */
int Syntax_AddBuiltInFunctions(Symtable *symtable)
{
    Element *function = NULL;
    if ( ( function = Symtable_CreateElement(symtable, "reads", FUNCTION)) == NULL)
        return -1;
    Element_AddReturn(function, SEMANTIC_STRING);
    Element_Define(function);

    if ( ( function = Symtable_CreateElement(symtable, "readi", FUNCTION)) == NULL)
        return -1;
    Element_AddReturn(function, SEMANTIC_INTEGER);
    Element_Define(function);

    if ( ( function = Symtable_CreateElement(symtable, "readn", FUNCTION)) == NULL)
        return -1;
    Element_AddReturn(function, SEMANTIC_NUMBER);
    Element_Define(function);

    if ( ( function = Symtable_CreateElement(symtable, "write", FUNCTION)) == NULL)
        return -1;
    Element_Define(function);

    if ( ( function = Symtable_CreateElement(symtable, "tointeger", FUNCTION)) == NULL)
        return -1;
    Element_AddParam(function, SEMANTIC_NUMBER, "f");
    Element_AddReturn(function, SEMANTIC_INTEGER);
    Element_Define(function);

    if ( ( function = Symtable_CreateElement(symtable, "substr", FUNCTION)) == NULL)
        return -1;
    Element_AddParam(function, SEMANTIC_STRING, "s");
    Element_AddParam(function, SEMANTIC_INTEGER, "i");
    Element_AddParam(function, SEMANTIC_INTEGER, "j");
    Element_AddReturn(function, SEMANTIC_STRING);
    Element_Define(function);

    if ( ( function = Symtable_CreateElement(symtable, "ord", FUNCTION)) == NULL)
        return -1;
    Element_AddParam(function, SEMANTIC_STRING, "s");
    Element_AddParam(function, SEMANTIC_NUMBER, "i");
    Element_AddReturn(function, SEMANTIC_INTEGER);
    Element_Define(function);

    if ( ( function = Symtable_CreateElement(symtable, "chr", FUNCTION)) == NULL)
        return -1;
    Element_AddParam(function, SEMANTIC_INTEGER, "i");
    Element_AddReturn(function, SEMANTIC_STRING);
    Element_Define(function);

    return 0;
}

/**
 * @brief Top to Bottom syntax method implemented as predictive parsing
 * @param input source code stream
 * @param output assembly output stream
 * @param error_output stream for everything else
 * @param clear whether this function was called only to clear stack
 * @return -1 upon work in progress
 *          1-9 & 99 upon an error
 *          0 upon successful finish
 */
int TopToBottom(FILE *input, FILE *output, FILE *error_output, bool clear)
{
    static Symtable *symtable = NULL;
    static Stack *topToBottomStack = NULL;
    static Token *token;
    static Buffers *buffer = NULL;
    static FSM_STATE state;

    if (clear)
    {
        Stack_Destroy(topToBottomStack);
        topToBottomStack = NULL;

        // maybe Token_Destroy in the future
        if (token != NULL)
            Token_Destroy(token);
        token = NULL;

        Vector_Destroy(buffer->variables);
        buffer->variables = NULL;

        Vector_Destroy(buffer->expressions);
        buffer->expressions = NULL;

        Symtable_Destroy(symtable);
        symtable = NULL;

        state = FSM_START;
        buffer->position = 0;
        buffer->declared = false;
        buffer->current_function = NULL;

        return 0;
    }

    // token and stack are NULLs
    if (topToBottomStack == NULL)
    {
        buffer = (Buffers*)malloc(sizeof(Buffers));
        if (buffer == NULL)
            return 99;

        buffer->variables = Vector_Init((void (*)(void*)) NULL);
        if (buffer->variables == NULL)
            return 99;

        buffer->expressions = Vector_Init((void (*)(void*)) NULL);
        if (buffer->expressions == NULL)
        {
            Vector_Destroy(buffer->variables);
            return 99;
        }

        symtable = Symtable_Init();
        if (symtable == NULL)
        {
            Vector_Destroy(buffer->variables);
            Vector_Destroy(buffer->expressions);
            return 99;
        }
        
        topToBottomStack = Stack_Init((void (*)(void*)) Symbol_Destroy);
        if (topToBottomStack == NULL)
        {
            Vector_Destroy(buffer->variables);
            Vector_Destroy(buffer->expressions);
            Symtable_Destroy(symtable);
            return 99;
        }

        Symbol *a = Symbol_Init(true, $);
        Stack_Push(topToBottomStack, a);

        // NT_PROGRAM is starting NonTerminal
        a = Symbol_Init(false, NT_PROGRAM);
        Stack_Push(topToBottomStack, a);

        token = getToken(input);

        state = FSM_START;
        buffer->position = 0;
        buffer->declared = false;
        buffer->current_function = NULL;

        // built-in-functions
        if (Syntax_AddBuiltInFunctions(symtable) != 0)
        {
            Vector_Destroy(buffer->variables);
            Vector_Destroy(buffer->expressions);
            Symtable_Destroy(symtable);
            return 99;
        }
        // for first non existing global_statement so we can allways pop scope
        Symtable_AddScope(symtable);
    }
    
    Symbol *top = (Symbol *) Stack_Top(topToBottomStack);

    #ifdef DEBUG_SYNTAX
        fprintf(error_output, "token [%d], stack top [%d, %d]\n", Token_getType(token), Symbol_IsTerminal(top), Symbol_GetValue(top));
        fprintf(error_output, "In state: %d\n", state);
    #endif
    
    if (Token_getType(token) == ERROR)
    {
        Token_Destroy(token);
        token = NULL;
        return 1;
    }

    int return_value = -1;

    if (Symbol_IsTerminal(top) && (Terminal)Symbol_GetValue(top) == $)
    {
        if (Token_getType(token) == $)
        {
            Token_Destroy(token);
            token = NULL;
            return 0;
        }
        else
        {
            Token_Destroy(token);
            token = NULL;
            return 2;
        }
    }
    else if (Symbol_IsTerminal(top))
    {
        if (Token_getType(token) == (Terminal) Symbol_GetValue(top))
        {
            Syntax_FSM_Action(&state, token, &return_value, symtable, buffer);

            Stack_Pop(topToBottomStack);
            Token_Destroy(token);
            token = getToken(input);
        }
        else
        {
            Token_Destroy(token);
            token = NULL;
            return 2;
        }
    }
    else
    {
        bool popScope = false;
        FSM_STATE previous_state = state;

        switch (Symbol_GetValue(top))
        {
        case NT_DECLARE:
            state = FSM_VAR_DEC;
            break;
        case NT_FUNCTION_DECLARE:
            state = FSM_FUN_DEC;
            break;
        case NT_FUNCTION_DEFINE:
            state = FSM_FUN_DEF;
            break;
        case NT_FUNCTION_CALL:
            state = FSM_FUN_CALL;
            break;
        case NT_ID:
            state = FSM_ID;
            break;
        case NT_RETURN:
            state = FSM_RETURNS;
            break;
        case NT_STATEMENT:
            state = FSM_START;
            break;

        // we must pop after all changes from previous scope are done
        case NT_GLOBAL_STATEMENT:
            state = FSM_START;
            popScope = true;
            break;
            
        // do nothing
        default:
            break;
        }

        if (previous_state != state)
        {
            switch (previous_state)
            {
            case FSM_VAR_ASSIGN:
                return_value = Syntax_Assign(buffer);
                break;
            case FSM_RETURNS:
                return_value = Syntax_Return(buffer);
                break;

            // add scope for current function
            case FSM_FUN_DEF_WAIT:
            case FSM_FUN_DEF_RETURNS:
                Symtable_AddScope(symtable);

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
                }
                break;
            
            default:
                break;
            }
            // clear Vectors
            Vector_Clear(buffer->variables);
            Vector_Clear(buffer->expressions);
            buffer->position = 0;
            buffer->declared = false;
        }

        if (popScope == true)
            if (buffer->current_function != NULL)
            {
                buffer->current_function = NULL;
                Symtable_PopScope(symtable);
            }

        if (Symbol_GetValue(top) == NT_EXPRESSION)
        {
            returnToken(token);
            Stack_Pop(topToBottomStack);
            
            Node* expression = NULL;
            int ret = BottomToTop(input, output, error_output, &expression, symtable);
            
            Vector_PushBack(buffer->expressions, expression);

            token = getToken(input);
            return ret;
        }

        // -1 in index to skip ERROR enum
        int index = LLTable[Symbol_GetValue(top)][Token_getType(token) - 1];
        
        if (index == -1)
        {
            Token_Destroy(token);
            token = NULL;
            return 2;
        }
        
        Stack_Pop(topToBottomStack);
        for (int i = Rule_GetSize(&(rules[index])) - 1; i >= 0; i--)
        {
            Symbol *next = Symbol_Init(Symbol_IsTerminal(Rule_GetSymbol(&(rules[index]), i)), Symbol_GetValue(Rule_GetSymbol(&(rules[index]), i)));

            if (Symbol_IsTerminal(next) && Symbol_GetValue(next) == EPSILON)
            {
                Symbol_Destroy(next);
                break;
            }

            Stack_Push(topToBottomStack, next);
        }

        #ifdef DEBUG_SYNTAX
            fprintf(error_output, "Apply new rule: %d\n", index);

            if (Symbol_IsTerminal(Rule_GetSymbol(&(rules[index]), 0)))
                fprintf(error_output, "<%d> -> %d", Symbol_GetValue(&(rules[index].left_side)), Symbol_GetValue(Rule_GetSymbol(&(rules[index]), 0)));
            else
                fprintf(error_output, "<%d> -> <%d>", Symbol_GetValue(&(rules[index].left_side)), Symbol_GetValue(Rule_GetSymbol(&(rules[index]), 0)));
            for (int i = 1 ; i < Rule_GetSize(&(rules[index])); i++)
            {
                if (Symbol_IsTerminal(Rule_GetSymbol(&(rules[index]), i)))
                    fprintf(error_output, ", %d", Symbol_GetValue(Rule_GetSymbol(&(rules[index]), i)));
                else
                    fprintf(error_output, ", <%d>", Symbol_GetValue(Rule_GetSymbol(&(rules[index]), i)));
            }
            fprintf(error_output, "\n");
        #endif
    }
    
    return return_value;
}

int parseAndGenerate(FILE *input, FILE *output, FILE *error_output)
{
    int return_value = 0;
    while (true)
    {
        int a = TopToBottom(input, output, error_output, false);
        
        // success
        if (a == 0)
            break;

        // error
        if (a > 0)
        {
            // only the first error is propagated as output value
            if (return_value == 0)
                return_value = a;
            break;
        }
    }
    TopToBottom(NULL, NULL, NULL, true);
    LexicalDestroy();

    // might move to main later
    #ifdef SYNTAX_DEBUG
        fprintf(error_output, "Exit code: %d\n", return_value);
    #endif

    return return_value;
}