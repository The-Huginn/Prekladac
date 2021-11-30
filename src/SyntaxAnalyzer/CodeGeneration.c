/**
 * @file CodeGeneration.c
 * @author Rastislav Budinsky
 * @brief This file implements functions generating code
 */
#include "CodeGeneration.h"
#include "SemanticActions.h"
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

int Code_AddCondition(Buffers *buffer, Node *expression, Symtable *symtable)
{
    Scope *top = Scope_Init(IF, buffer->top_id++);
    if (top == NULL)
        return 99;

    if (expression == NULL)
        return 99;

    Stack_Push(buffer->scopes, top);

    if (Node_GetSemantic(expression) != SEMANTIC_BOOLEAN)
    {
        if (AbstractSemanticTree_CompareWithNil(expression) == false)
            return 7;
    }

    Vector *condition = Node_PostOrder(expression, true, buffer, 1);

    if (Vector_IsEmpty(condition))
    {
        WARNING("Expecting expression in condition");
        Vector_Destroy(condition);
        return 7;
    }

    fprintf(buffer->output, "JUMPIFNEQ $if%d_%d LF@%s%d bool@true\n", top->id, top->label_count, TMP(*((int*)Vector_GetElement(condition, 0))));

    Vector_Destroy(condition);

    Symtable_AddScope(symtable);

    return -1;
}

int Code_AddElseif(Buffers *buffer, Node *expression, Symtable *symtable)
{
    Scope *top = Stack_Top(buffer->scopes);
    if (top == NULL)
        return 99;

    if (expression == NULL)
        return 99;
    
    if (Node_GetSemantic(expression) != SEMANTIC_BOOLEAN)
    {
        if (AbstractSemanticTree_CompareWithNil(expression) == false)
            return 7;
    }

    Vector *condition = Node_PostOrder(expression, true, buffer, 1);

    if (Vector_IsEmpty(condition))
    {
        WARNING("Expecting expression in condition");
        Vector_Destroy(condition);
        return 7;
    }

    fprintf(buffer->output, "LABEL $if%d_%d\n", top->id, top->label_count++); // increases label for next one
    fprintf(buffer->output, "JUMPIFNEQ $if%d_%d LF@%s%d bool@true\n", top->id, top->label_count, TMP(*((int*)Vector_GetElement(condition, 0))));

    Vector_Destroy(condition);

    Symtable_AddScope(symtable);

    return -1;
}

int Code_AddElse(Buffers *buffer, Symtable *symtable)
{
    Scope *top = Stack_Top(buffer->scopes);
    if (top == NULL)
        return 99;

    top->last_else = true;

    fprintf(buffer->output, "LABEL $if%d_%d\n", top->id, top->label_count);

    Symtable_AddScope(symtable);

    return -1;
}

int Code_AddWhile(Buffers *buffer, Node *expression, Symtable *symtable)
{
    Scope *top = Scope_Init(WHILE, buffer->top_id++);
    if (top == NULL)
        return 99;

    if (expression == NULL)
        return 99;

    Stack_Push(buffer->scopes, top);

    // we increase label count for end while
    fprintf(buffer->output, "LABEL $while%d_%d", top->id, top->label_count++);

    // we need to generate expression code after label as it needs to be executed every time
    if (Node_GetSemantic(expression) != SEMANTIC_BOOLEAN)
    {
        if (AbstractSemanticTree_CompareWithNil(expression) == false)
            return 7;
    }

    Vector *condition = Node_PostOrder(expression, true, buffer, 1);

    if (Vector_IsEmpty(condition))
    {
        WARNING("Expecting expression in condition");
        Vector_Destroy(condition);
        return 7;
    }

    fprintf(buffer->output, "JUMPIFNEQ $while%d_%d LF@%s%d bool@true\n", top->id, top->label_count, TMP(*((int*)Vector_GetElement(condition, 0))));

    Vector_Destroy(condition);

    Symtable_AddScope(symtable);

    return -1;
}

int Code_AddFunction(Buffers *buffer)
{
    // id not important for function
    Scope *function_scope = Scope_Init(FUNCTION_DEF, -1);
    if (function_scope == NULL)
        return 99;

    Stack_Push(buffer->scopes, function_scope);

    fprintf(buffer->output, "LABEL %s%d\n", ELEMENT(buffer->current_function));
    fprintf(buffer->output, "CREATEFRAME\n");
    fprintf(buffer->output, "PUSHFRAME\n");

    for (int i = 0; i < Vector_Size(buffer->variables); i++)
    {
        fprintf(buffer->output, "DEFVAR LF@%s%d\n", ELEMENT(Vector_GetElement(buffer->variables, i)));
        fprintf(buffer->output, "POPS LF@%s%d\n", ELEMENT(Vector_GetElement(buffer->variables, i)));
    }

    return -1;
}

void Code_DeclareVariables(Buffers *buffer)
{
    for (int i = 0; i < Vector_Size(buffer->variables); i++)
        fprintf(buffer->output, "DEFVAR LF@%s%d\n", ELEMENT(Vector_GetElement(buffer->variables, i)));
}

/**
 * @brief Generates code for all expressions and returns Vector of numbers in order from left to right where in TMP() are saved
 * @param buffer Buffers
 * @param assignment true if we are assigning values false if we are returning value
 *      @note important to know if last expression is function call and we need to know how much more we need
 * @return Vector of int*
 */
Vector *Code_GetRValues(Buffers *buffer, bool assignment)
{
    Vector *returns = Vector_Init(Number_Destroy);

    for (int i = 0; i < Vector_Size(buffer->expressions); i++)
    {
        Vector* tmp = NULL;
        if (i == Vector_Size(buffer->expressions) - 1)
            tmp = Node_PostOrder(
                (Node*)Vector_GetElement(buffer->expressions, i),
                true,
                buffer,
                (assignment ? Vector_Size(buffer->variables) : Element_FunctionReturns_Size((buffer->current_function))) - Vector_Size(buffer->expressions) + 1
            );
        else
            tmp = Node_PostOrder(
                (Node*)Vector_GetElement(buffer->expressions, i),
                true,
                buffer,
                1
            );

        if (tmp == NULL)
            ERROR("Missing expressions");
        
        if (!Vector_IsEmpty(tmp))
            buffer->tmp_offset = *((int*)Vector_Back(tmp)) + 1;

        for (int i = 0; i < Vector_Size(tmp); i++)
            Vector_PushBack(returns, Number_Init(*((int*)Vector_GetElement(tmp, i))));
    }

    return returns;
}

void Code_GenerateAssign(Buffers *buffer)
{
    Vector *assignments = Code_GetRValues(buffer, true);
    if (assignments == NULL)
        return;

    for (int i = Vector_Size(buffer->variables) - 1; i >= 0; i--)
        fprintf(buffer->output, "MOVE %s%d %s%d\n", ELEMENT(Vector_GetElement(buffer->variables, i)), TMP(*((int*)Vector_GetElement(assignments, i))));

    Vector_Destroy(assignments);
}

void Code_GenerateFunctionCall(Buffers *buffer, Node *function_call)
{
    Node_PostOrder(function_call, true, buffer, 0);
}

void Code_GenerateFunctionReturn(Buffers *buffer)
{
    Vector *returns = Code_GetRValues(buffer, false);
    if (returns == NULL)
        return;

    for (int i = Vector_Size(returns) - 1; i >= 0; i--)
        fprintf(buffer->output, "PUSHS %s%d\n", TMP(*((int*)Vector_GetElement(returns, i))));

    Vector_Destroy(returns);

    fprintf(buffer->output, "POPFRAME\n");
    fprintf(buffer->output, "RETURN\n");
}

void Code_PopEnd(Buffers *buffer, Symtable *symtable)
{
    Scope *top = (Scope*)Stack_Top(buffer->scopes);
    switch (top->type)
    {
    case IF:
        // we need to generate label for "empty else"
        if (top->last_else == false)
            fprintf(buffer->output, "LABEL $if%d_%d\n", top->id, top->label_count);
        break;
    case WHILE:
        fprintf(buffer->output, "JUMP $while%d_%d\n", top->id, (top->label_count - 1));
        fprintf(buffer->output, "LABEL $while%d_%d\n", top->id, top->label_count);
        break;
    case FUNCTION_DEF:
        fprintf(buffer->output, "POPFRAME\n");
        fprintf(buffer->output, "RETURN\n");
        buffer->current_function = NULL;
        buffer->tmp_offset = 0;
        break;
    
    default:
        break;
    }

    Stack_Pop(buffer->scopes);
    Symtable_PopScope(symtable);
}