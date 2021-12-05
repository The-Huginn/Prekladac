// IFJ Project 2021
/**
 * @file GenerateASS.c
 * @author Rastislav Budinsky (xbudin05)
 * @brief This file implements functions generating code
 */
#include "GenerateASS.h"
#include "SemanticActions.h"
#include "../Utils/logger.h"

#include <stdlib.h>

typedef enum {IF, WHILE, FUNCTION_DEF, FOR, REPEAT}ScopeItem;

typedef struct
{
    ScopeItem type;
    bool last_else;
    Node *node;
    int id;         // to know the id of loop scope we are currently in
}Scope;

Scope *Scope_Init(ScopeItem type, int id)
{
    Scope *scope = (Scope*)malloc(sizeof(Scope));
    
    if (scope == NULL)
        ERROR("Allocation failed");

    scope->type = type;
    scope->last_else = false;
    scope->node = NULL;
    scope->id = id;

    return scope;
}

void ASS_AddHeader(Buffers *buffer)
{
    fprintf(buffer->output, ".IFJcode21\n");
    fprintf(buffer->output, "JUMP %s\n", JUMP);
    FILE *builtIn = fopen("../../../../src/files/BuiltInFunctions.txt", "r");
    if (builtIn != NULL)
    {
        int a;
        while ((a = getc(builtIn)) != EOF)
            fprintf(buffer->output, "%c", a);
    }
    fprintf(buffer->output, "\n#COMPILED CODE BEGINS HERE\n\n");
}

void ASS_AddFooter(Buffers *buffer)
{
    fprintf(buffer->output, "LABEL %s\n", JUMP);
    fprintf(buffer->output, "CREATEFRAME\n");
    fprintf(buffer->output, "PUSHFRAME\n");
    for (int i = 0; i < Vector_Size(buffer->function_calls); i++)
        Vector_Destroy(Node_PostOrder(Vector_GetElement(buffer->function_calls, i), true, buffer, 0, ALL));
    fprintf(buffer->output, "POPFRAME\n");
}

int ASS_AddCondition(Buffers *buffer, Node *expression, Symtable *symtable)
{
    if (expression == NULL)
        return 99;

    Scope *top = Stack_Top(buffer->scopes);
    
    top = Scope_Init(IF, top->id);
    if (top == NULL)
        return 99;

    Stack_Push(buffer->scopes, top);

    if (Node_GetSemantic(expression) != SEMANTIC_BOOLEAN)
    {
        if (AbstractSemanticTree_CompareWithNil(expression) == false)
            return 7;
    }

    top->node = Node_Init(NODE_IF, expression, SEMANTIC_VOID, (void (*)(void *))NULL, P_VOID);
    if (top->node == NULL)
        return 99;

    Node_SetParamCount(top->node, buffer->top_id++);

    Symtable_AddScope(symtable);

    return -1;
}

int ASS_AddElseif(Buffers *buffer, Node *expression, Symtable *symtable)
{
    Scope *top = (Scope*)Stack_Top(buffer->scopes);
    if (top == NULL)
        return 99;

    if (expression == NULL)
        return 99;
    
    if (Node_GetSemantic(expression) != SEMANTIC_BOOLEAN)
    {
        if (AbstractSemanticTree_CompareWithNil(expression) == false)
            return 7;
    }

    Node *node = Node_Init(NODE_ELSEIF, expression, SEMANTIC_VOID, (void (*)(void *))NULL, P_VOID);

    if (node == NULL)
        return 99;

    Node_AppendSon(top->node, node);

    Symtable_AddScope(symtable);

    return -1;
}

int ASS_AddElse(Buffers *buffer, Symtable *symtable)
{
    Scope *top = (Scope*)Stack_Top(buffer->scopes);
    if (top == NULL)
        return 99;

    top->last_else = true;

    Node *node = Node_Init(NODE_ELSE, NULL, SEMANTIC_VOID, (void (*)(void *))NULL, P_VOID);
    if (node == NULL)
        return 99;

    Node_AppendSon(top->node, node);

    Symtable_AddScope(symtable);

    return -1;
}

int ASS_AddWhile(Buffers *buffer, Symtable *symtable)
{
    Scope *top = Scope_Init(WHILE, buffer->top_id);
    if (top == NULL)
        return 99;

    if (Vector_Size(buffer->expressions) != 1)
        return 99;

    Node *expression = Vector_Back(buffer->expressions);

    Stack_Push(buffer->scopes, top);

    // we need to generate expression code after label as it needs to be executed every time
    if (Node_GetSemantic(expression) != SEMANTIC_BOOLEAN)
    {
        if (AbstractSemanticTree_CompareWithNil(expression) == false)
            return 7;
    }

    Node *node = Node_Init(NODE_WHILE, expression, SEMANTIC_VOID, (void (*)(void *))NULL, P_VOID);

    if (node == NULL)
        return 99;

    top->node = node;
    Node_SetParamCount(top->node, buffer->top_id++);

    Symtable_AddScope(symtable);

    return -1;
}

int ASS_AddFor(Buffers *buffer)
{
    Scope *top = Scope_Init(FOR, buffer->top_id);
    if (top == NULL)
        return 99;

    if (Vector_Size(buffer->expressions) != 3 || Vector_Size(buffer->variables) != 1)
        return 99;

    Stack_Push(buffer->scopes, top);

    // data is variable Element
    Node *node = Node_Init(NODE_FOR, Vector_Back(buffer->variables), SEMANTIC_VOID, (void (*)(void *))NULL, P_VOID);

    if (node == NULL)
        return 99;

    top->node = node;
    Node_SetParamCount(top->node, buffer->top_id++);

    // expressions of for loop
    for (int i = 0; i < Vector_Size(buffer->expressions); i++)
        Node_AppendSon(node, Vector_GetElement(buffer->expressions, i));

    return -1;
}

int ASS_AddRepeat(Buffers *buffer, Symtable *symtable)
{
    Scope *top = Scope_Init(REPEAT, buffer->top_id);
    if (top == NULL)
        return 99;

    Stack_Push(buffer->scopes, top);

    // we add data as expression later
    Node *node = Node_Init(NODE_REPEAT, NULL, SEMANTIC_VOID, (void (*)(void *))NULL, P_VOID);

    if (node == NULL)
        return 99;

    top->node = node;
    Node_SetParamCount(top->node, buffer->top_id++);

    Symtable_AddScope(symtable);

    return -1;
}

int ASS_AddBreak(Buffers *buffer)
{
    Scope *top = (Scope*)Stack_Top(buffer->scopes);

    // break is not in loop
    if (top->id == -1)
        return 7;

    Node *node = Node_Init(NODE_BREAK, NULL, SEMANTIC_VOID, (void (*)(void *))NULL, P_VOID);
    Node_SetParamCount(node, top->id);

    if (node == NULL)
        return 99;

    Node_AppendSon(top->node, node);

    return -1;
}

int ASS_AddFunction(Buffers *buffer)
{
    // -1 so we can detect if we get break outside loops
    Scope *function_scope = Scope_Init(FUNCTION_DEF, -1);
    if (function_scope == NULL)
        return 99;

    Stack_Push(buffer->scopes, function_scope);

    Node *node = Node_Init(NODE_FUNCTION_DEF, buffer->current_function, SEMANTIC_VOID, (void (*)(void *))NULL, P_VOID);

    if (node == NULL)
        return 99;

    function_scope->node = node;

    for (int i = 0; i < Vector_Size(buffer->variables); i++)
    {
        Node *son = Node_Init(NODE_FUNCTION_POP, Vector_GetElement(buffer->variables, i), SEMANTIC_VOID, (void (*)(void *))NULL, P_VOID);
        if (son == NULL)
            return 99;

        Node_AppendSon(node, son);
    }

    return -1;
}

void ASS_DeclareVariables(Buffers *buffer)
{
    Scope *top = (Scope*)Stack_Top(buffer->scopes);

    if (top == NULL)
        return;
    
    for (int i = 0; i < Vector_Size(buffer->variables); i++)
    {
        Node *node = Node_Init(NODE_DECLARE, Vector_GetElement(buffer->variables, i), SEMANTIC_VOID, (void (*)(void *))NULL, P_VOID);

        if (node == NULL)
            return;

        Node_AppendSon(top->node, node);
    }
}

// we create 2 child node one for lvalues one for rvalues and parent node as assigment node
void ASS_GenerateAssign(Buffers *buffer)
{
    Scope *top = (Scope*)Stack_Top(buffer->scopes);

    if (top == NULL)
        return;

    Node *node = Node_Init(NODE_ASSIGN, NULL, SEMANTIC_VOID, (void (*)(void *))NULL, P_VOID);

    if (node == NULL)
        return;

    Node *lvalues = Node_Init(NODE_ASSIGN, NULL, SEMANTIC_VOID, (void (*)(void *))NULL, P_VOID);
    if (lvalues == NULL)
    {
        Node_Destroy(node, false);
        return;
    }

    Node *rvalues = Node_Init(NODE_ASSIGN, NULL, SEMANTIC_VOID, (void (*)(void *))NULL, P_VOID);
    if (rvalues == NULL)
    {
        Node_Destroy(node, false);
        Node_Destroy(lvalues, false);
        return;
    }

    // as sons we generate NODE_ID, as returns we append expressions
    //  note do not use stantard NODE_ID operation upon them

    for (int i = 0; i < Vector_Size(buffer->variables); i++)
    {
        Node *son = Node_Init(NODE_ID, Vector_GetElement(buffer->variables, i), SEMANTIC_VOID, (void (*)(void *))NULL, P_VOID);

        if (son == NULL)
            return;

        Node_AppendSon(lvalues, son);
    }

    for (int i = 0; i < Vector_Size(buffer->expressions); i++)
        Node_AppendSon(rvalues, (Node*)Vector_GetElement(buffer->expressions, i));

    Node_AppendSon(node, lvalues);
    Node_AppendSon(node, rvalues);

    Node_AppendSon(top->node, node);
}

void ASS_GenerateFunctionReturn(Buffers *buffer)
{
    Scope *top = (Scope*)Stack_Top(buffer->scopes);

    if (top == NULL)
        return;

    Node *node = Node_Init(NODE_RETURN, NULL, SEMANTIC_VOID, (void (*)(void *))NULL, P_VOID);

    if (node == NULL)
        return;

    for (int i = 0; i < Vector_Size(buffer->expressions); i++)
        Node_AppendSon(node, (Node*)Vector_GetElement(buffer->expressions, i));

    Node_AppendSon(top->node, node);
}

void ASS_GenerateFunctionCall(Buffers *buffer, Node *function_call)
{
    // global function call
    if (Stack_IsEmpty(buffer->scopes))
    {
        Vector_PushBack(buffer->function_calls, function_call);
    }
    // local function call
    else
    {
        Scope *top = (Scope*)Stack_Top(buffer->scopes);

        Node_AppendSon(top->node, function_call);
    }

    return;
}

int ASS_PopEnd(Buffers *buffer, Symtable *symtable)
{
    Scope *top = (Scope*)Stack_Top(buffer->scopes);
    Node *node = top->node;

    bool node_above = true;

    switch (top->type)
    {
    case IF:
        // we need to generate label for "empty else"
        if (top->last_else == false)
            Node_AppendSon(top->node, Node_Init(NODE_ELSE, NULL, SEMANTIC_VOID, (void (*)(void *))NULL, P_VOID));
        break;

    case WHILE:
        break;

    case FOR:
        break;

    case REPEAT:
        if (Vector_Size(buffer->expressions) != 1)
            return 99;
        
        // we need to generate expression code after label as it needs to be executed every time
        if (Node_GetSemantic(Vector_Back(buffer->expressions)) != SEMANTIC_BOOLEAN)
        {
            if (AbstractSemanticTree_CompareWithNil(Vector_Back(buffer->expressions)) == false)
                return 7;
        }

        Node_SetData(node, Vector_Back(buffer->expressions));
        break;
    
    case FUNCTION_DEF:
        node_above = false;

        Vector_Destroy(Node_PostOrder(top->node, true, buffer, 0, ALL));
        
        buffer->current_function = NULL;
        buffer->tmp_offset = 0;
        break;
    
    default:
        break;
    }

    Stack_Pop(buffer->scopes);

    // append inner statement
    if (node_above == true)
    {
        // we should not destroy elements before code is generated
        Symtable_PopScopeToBuffer(symtable);
        top = (Scope*)Stack_Top(buffer->scopes);
        Node_AppendSon(top->node, node);
    }
    else
    {
        Symtable_PopScope(symtable);
        Symtable_ClearBuffer(symtable);
    }

    return -1;
}