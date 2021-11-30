/**
 * @file CodeGeneration.c
 * @author Rastislav Budinsky
 * @brief This file implements functions generating code
 */
#include "CodeGeneration.h"
#include "../Utils/logger.h"

#include <stdlib.h>

typedef enum {IF, WHILE, FUNCTION_DEF}ScopeItem;

typedef struct
{
    ScopeItem type;
    int id;
    int label_count;
    bool last_else;
}Scope;

Scope *Scope_Init( ScopeItem type, int unique_id)
{
    Scope *scope = (Scope*)malloc(sizeof(Scope));
    
    if (scope == NULL)
        ERROR("Allocation failed");

    scope->type = type;
    scope->id = unique_id;
    scope->label_count = 0;
    scope->last_else = false;

    return scope;
}

void Code_AddHeader(Buffers *buffer)
{
    fprintf(buffer->output, ".IFJcode21\n");
}

void Code_AddCondition(Buffers *buffer, Node *expression)
{
    Scope *if_scope = Scope_Init(IF, buffer->top_id++);
    if (if_scope == NULL)
        return;

    Stack_Push(buffer->scopes, if_scope);

    // TODO generate expression, check whether bool or adjust
}

void Code_AddElseif(Buffers *buffer, Node *expression)
{
    // TODO generate expression, check whether bool or adjust
}

void Code_AddElse(Buffers *buffer)
{
    ((Scope*)Stack_Top(buffer->scopes))->last_else = true;
    // TODO generate code
}

void Code_AddWhile(Buffers *buffer, Node *expression)
{
    Scope *while_scope = Scope_Init(WHILE, buffer->top_id++);
    if (while_scope == NULL)
        return;

    Stack_Push(buffer->scopes, while_scope);

    // TODO generate expression, check whether bool or adjust
}

void Code_AddFunction(Buffers *buffer)
{
    // id not important for function
    Scope *function_scope = Scope_Init(FUNCTION_DEF, -1);
    if (function_scope == NULL)
        return;

    Stack_Push(buffer->scopes, function_scope);

    // TODO generate create frame and pop values from buffer->currenct_function
}


void Code_DeclareVariables(Buffers *buffer)
{
    for (int i = 0; i < Vector_Size(buffer->variables); i++)
        fprintf(buffer->output, "DEFVAR LF@%s%d\n", ELEMENT(Vector_GetElement(buffer->variables, i)));
}

void Code_GenerateAssign(Buffers *buffer)
{
    Vector *assignments = Vector_Init(Number_Destroy);
    
    for (int i = 0; i < Vector_Size(buffer->expressions); i++)
    {
        Vector* variables = NULL;
        if (i == Vector_Size(buffer->expressions) - 1)
            variables = Node_PostOrder(
                (Node*)Vector_GetElement(buffer->expressions, i),
                true,
                buffer,
                Vector_Size(buffer->variables) - Vector_Size(buffer->expressions) + 1,
                buffer->output
            );
        else            
            variables = Node_PostOrder(
                (Node*)Vector_GetElement(buffer->expressions, i),
                true,
                buffer,
                1,
                buffer->output
            );

        if (variables == NULL)
            ERROR_VOID("Missing expressions");
        
        if (!Vector_IsEmpty(variables))
            buffer->tmp_offset = *((int*)Vector_Back(variables)) + 1;

        for (int i = 0; i < Vector_Size(variables); i++)
            Vector_PushBack(assignments, Number_Init(*((int*)Vector_GetElement(variables, i))));
    }

    for (int i = Vector_Size(buffer->variables) - 1; i >= 0; i--)
    {
        fprintf(buffer->output, "MOVE %s%d %s%d\n", ELEMENT(Vector_GetElement(buffer->variables, i)), TMP(*((int*)Vector_GetElement(assignments, i))));
    }

    Vector_Destroy(assignments);
}

void Code_GenerateFunctionCall(Buffers *buffer, Node *function_call)
{
    Node_PostOrder(function_call, true, buffer, 0, buffer->output);
}

void Code_GenerateFunctionReturn(Buffers *buffer)
{
    // TODO generate code
}

void Code_PopEnd(Buffers *buffer)
{
    Scope *top = (Scope*)Stack_Top(buffer->scopes);
    switch (top->type)
    {
    case IF:
        break;
    case WHILE:
        break;
    case FUNCTION_DEF:
        buffer->current_function = NULL;
        break;
    
    default:
        break;
    }

    Stack_Pop(buffer->scopes);
}