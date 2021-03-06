// IFJ Project 2021
/**
 * @file SyntaxAnalyzer.c
 * @brief This file implements interface of SyntaxAnalyzer.h, contains custom functions
 * @author Rastislav Budinsky (xbudin05)
 */
#include "SyntaxAnalyzer.h"
#include "Terminal.h"
#include "NonTerminal.h"
#include "LLTable.h"
#include "BottomToTop.h"
#include "SemanticActions.h"
#include "SyntaxFunctions.h"
#include "GenerateASS.h"

#include "../Utils/stack.h"
#include "../Utils/list.h"
#include "../Utils/vector.h"
#include "../Utils/symtable.h"
#include "../Utils/Buffers.h"

#include "../LexicalAnalyzer/LexicalAnalyzer.h"


#include <stdlib.h>

typedef enum {FSM_START, FSM_VAR_DEC, FSM_VAR_DATATYPES, FSM_VAR_ASSIGN, FSM_ID, FSM_FUN_CALL, FSM_FUN_PARAMS, FSM_FUN_DEF, FSM_FUN_DEF_PARAMS,
    FSM_FUN_DEF_WAIT, FSM_FUN_DEF_RETURNS, FSM_FUN_DEC, FSM_FUN_DEC_PARAMS, FSM_FUN_DEC_WAIT, FSM_FUN_DEC_RETURNS, FSM_RETURNS, FSM_IF, FSM_ELSEIF,
    FSM_ELSE, FSM_WHILE, FSM_FOR, FSM_REPEAT, FSM_UNTIL}FSM_STATE;

// #define DEBUG_SYNTAX

/**
 * @brief Decides what is our next internal state of FSM about Syntax and Semantic validation with code generation combined
 * @note we rely on LL-grammar so we dont need to be deterministic so in case of validating wrong code LL-grammar will end SyntaxAnalyzer
 * @param state FSM_STATE pointer
 * @param token Token
 * @param return_value return_value int pointer
 * @param symtable Symtable
 * @param buffer Buffers
 * @return -1 upon success otherwise error code in return_value
 */
void Syntax_FSM_Action(FSM_STATE *state, Token *token, int *return_value, Symtable *symtable, Buffers *buffer)
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

    case FSM_VAR_DATATYPES:     // if we get '=' we go to assign, if we get datatype or ',' we stay and update variables otherwise START
        if (Token_getType(token) == T_ASS)
        {
            // we have not declared all variables properly
            if (buffer->position != Vector_Size(buffer->variables))
                *return_value = 2;
            *state = FSM_VAR_ASSIGN;
            if (*return_value == -1)
                ASS_DeclareVariables(buffer);
        }
        else if (Syntax_IsDatatype(token))
            *return_value = Syntax_Variable_SetSemantic(buffer, token);
        else if (Token_getType(token) != T_COMMA)
        {
            ASS_DeclareVariables(buffer);
            *state = FSM_START;
            Buffers_Clear(buffer);
        }
        break;

    case FSM_VAR_ASSIGN: // unless we get another terminal to pop other than ',' we assign expressions, we should not get into this state when in expression
        if (Token_getType(token) != T_COMMA)
        {
            *return_value = Syntax_Variable_Assign(buffer);
            *state = FSM_START;
            Buffers_Clear(buffer);
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
        else if (Token_getType(token) != T_COMMA)   // should be detected as syntax error
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
            Buffers_Clear(buffer);
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
            Vector_PushBack(buffer->only_declared, function);
        }
        else if (Token_getType(token) == T_LEFT)
            *state = FSM_FUN_DEC_PARAMS;
        break;

    case FSM_FUN_DEC_PARAMS:
        if (Token_getType(token) == T_RIGHT)
            *state = FSM_FUN_DEC_WAIT;
        else if (Syntax_IsDatatype(token))
            Element_AddParam((Element*)Vector_Back(buffer->variables), Syntax_GetSemantic(token), NULL);
        break;

    case FSM_FUN_DEC_WAIT:
        if (Token_getType(token) == T_DEF)
            *state = FSM_FUN_DEC_RETURNS;
        else
        {
            *state = FSM_START;
            Buffers_Clear(buffer);
        }
        break;

    case FSM_FUN_DEC_RETURNS:
        if (Syntax_IsDatatype(token))
            Element_AddReturn((Element*)Vector_Back(buffer->variables), Syntax_GetSemantic(token));
        else if (Token_getType(token) != T_COMMA)
        {
            Syntax_Return_Assign(buffer);
            *state = FSM_START;
            Buffers_Clear(buffer);
        }
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
        else if (Token_getType(token) == T_ID) // we increment buffer->position when we check Semantic type
        {
            if (buffer->declared == true)
                Element_FunctionParameter_SetName((Element*)Vector_Back(buffer->variables), buffer->position, (const char*)Token_getData(token));
            else
                Element_AddParam((Element*)Vector_Back(buffer->variables), SEMANTIC_VOID, (const char*)Token_getData(token));
        }
        else if (Syntax_IsDatatype(token))
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
            *return_value = Syntax_FunctionDefined(buffer, symtable);
        break;

    case FSM_FUN_DEF_RETURNS:
        if (Syntax_IsDatatype(token))
        {
            if (buffer->declared == true)
                *return_value = Syntax_GetSemantic(token) == Element_FunctionReturn_GetSemantic((Element*)Vector_Back(buffer->variables), (buffer->position)++) ? -1 : 3;
            else
                Element_AddReturn((Element*)Vector_Back(buffer->variables), Syntax_GetSemantic(token));
        }
        else if (Token_getType(token) != T_COMMA)
        {
            *state = FSM_START;
            Buffers_Clear(buffer);
        }
        break;

    case FSM_RETURNS:

        // ignore 'return'
        if (Token_getType(token) == K_RETURN)
            break;
        if (Token_getType(token) != T_COMMA)
        {
            *return_value = Syntax_Return_Assign(buffer);
            *state = FSM_START;
            Buffers_Clear(buffer);
        }
        break;

    case FSM_IF:
        if (!Vector_IsEmpty(buffer->expressions))
        {
            ASS_AddCondition(buffer, Vector_Back(buffer->expressions), symtable);
            *state = FSM_START;
            Buffers_Clear(buffer);
        }
        break;
    
    case FSM_ELSEIF:
        if (!Vector_IsEmpty(buffer->expressions))
        {
            ASS_AddElseif(buffer, Vector_Back(buffer->expressions), symtable);
            *state = FSM_START;
            Buffers_Clear(buffer);
        }
        break;

    case FSM_WHILE:
        if (Vector_IsEmpty(buffer->expressions))
            break;

        *return_value = ASS_AddWhile(buffer, symtable);
        *state = FSM_START;
        Buffers_Clear(buffer);
        break;

    case FSM_UNTIL:
        // we have to do this upon changed statement
        break;

    case FSM_FOR:
        if (Token_getType(token) == T_ID)
        {
            Element *element = Symtable_CreateElement(symtable, Token_getData(token), VARIABLE);
            Element_SetSemantic(element, SEMANTIC_INTEGER);
            Element_Define(element);
            Vector_PushBack(buffer->variables, element);
        }

        if (Vector_Size(buffer->expressions) != 3)
            break;

        *return_value = ASS_AddFor(buffer);

        *state = FSM_START;
        Buffers_Clear(buffer);
        break;

    default:
        break;
    }

    switch (Token_getType(token))
    {
    case K_IF:
        *state = FSM_IF;
        break;
    case K_ELSEIF:
        *state = FSM_ELSEIF;
        break;
    case K_ELSE:
        if (*return_value == -1)
            *return_value = ASS_AddElse(buffer, symtable);
        break;
    case K_WHILE:
        *state = FSM_WHILE;
        break;
    case K_END:
        if (*return_value == -1)
            *return_value = ASS_PopEnd(buffer, symtable);
        break;
    case K_FOR:
        *state = FSM_FOR;
        Symtable_AddScope(symtable);
        break;
    case K_REPEAT:
        if (*return_value == -1)
            *return_value = ASS_AddRepeat(buffer, symtable);
        break;
    case K_BREAK:
        if (*return_value == -1)
            *return_value = ASS_AddBreak(buffer);
        break;
    case K_UNTIL:
        *state = FSM_UNTIL;
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
        ASS_AddFooter(buffer);
        
        int return_value = 0;

        Stack_Destroy(topToBottomStack);
        topToBottomStack = NULL;

        if (token != NULL)
            Token_Destroy(token);
        token = NULL;

        // we first check if all declared function have been defined as well
        while (!Vector_IsEmpty(buffer->only_declared))
        {
            if (Element_IsDefined((Element*)Vector_Back(buffer->only_declared)) == false)
                return_value = 3;

            Vector_PopBack(buffer->only_declared);
        }

        Buffers_Destroy(buffer);
        buffer = NULL;

        Symtable_Destroy(symtable);
        symtable = NULL;

        state = FSM_START;

        return return_value;
    }

    // token and stack are NULLs
    if (topToBottomStack == NULL)
    {
        buffer = Buffers_Init(output);
        if (buffer == NULL)
            return 99;

        symtable = Symtable_Init();
        if (symtable == NULL)
        {
            Buffers_Destroy(buffer);
            return 99;
        }
        
        topToBottomStack = Stack_Init((void (*)(void*)) Symbol_Destroy);
        if (topToBottomStack == NULL)
        {
            Buffers_Destroy(buffer);
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

        // built-in-functions
        if (Syntax_AddBuiltInFunctions(symtable) != 0)
        {
            Buffers_Destroy(buffer);
            Symtable_Destroy(symtable);
            return 99;
        }

        ASS_AddHeader(buffer);
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

        case NT_GLOBAL_STATEMENT:
            state = FSM_START;
            break;
            
        default:
            break;
        }

        // new statement
        if (previous_state != state)
        {
            switch (previous_state)
            {
            case FSM_VAR_ASSIGN:
                return_value = Syntax_Variable_Assign(buffer);
                break;
            case FSM_RETURNS:
                if (buffer->current_function != NULL)
                    return_value = Syntax_Return_Assign(buffer);
                break;
            case FSM_VAR_DATATYPES:
                ASS_DeclareVariables(buffer);
                break;

            case FSM_UNTIL:
                if (Vector_IsEmpty(buffer->expressions))
                    return_value = 2;
                else
                {
                    if (Node_GetSemantic(Vector_Back(buffer->expressions)) != SEMANTIC_BOOLEAN)
                        return_value = AbstractSemanticTree_CompareWithNil(Vector_Back(buffer->expressions));

                    if (return_value == -1)
                        return_value = ASS_PopEnd(buffer, symtable);
                }
                break;        

            // add scope for current function
            case FSM_FUN_DEF_WAIT:
            case FSM_FUN_DEF_RETURNS:

                if (buffer->current_function != NULL)
                    return_value = Syntax_FunctionDefined(buffer, symtable);

                break;
            
            default:
                break;
            }

            // clear Vectors
            Buffers_Clear(buffer);
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
    int ret = TopToBottom(NULL, NULL, NULL, true);
    if (return_value == 0)
        return_value = ret;

    LexicalDestroy();

    // might move to main later
    #ifdef SYNTAX_DEBUG
        fprintf(error_output, "Exit code: %d\n", return_value);
    #endif

    return return_value;
}