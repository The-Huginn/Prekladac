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
    // TODO generate code
}

void Code_GenerateAssign(Buffers *buffer)
{
    // TODO generate code
}

void Code_GenerateFunctionCall(Buffers *buffer)
{
    // TODO generate code
}

void Code_GenerateFunctionReturn(Buffers *buffer)
{
    // TODO generate code
}

void Code_PopEnd(Buffers *buffer)
{
    Scope *top = Stack_Top(buffer->scopes);
    fprintf(stderr, "Clearing from end!\n");
    switch (top->type)
    {
    case IF:
        break;
    case WHILE:
        break;
    case FUNCTION_DEF:
        fprintf(stderr, "Clearing function\n");
        buffer->current_function = NULL;
        break;
    
    default:
        break;
    }

    Stack_Pop(buffer->scopes);
}