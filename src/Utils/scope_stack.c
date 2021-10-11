/**
 * @file scope_stack.c
 * @brief This file implements interface of scope_stack.h
 * @author Rastislav Budinsky
 */
#include "scope_stack.h"
#include "logger.h"

typedef struct SStackElement
{
    ScopeList *data;
    struct SStackElement *next;
}SSElement;

SSElement *SSElement_Init(SSElement *next)
{
    SSElement *element = (SSElement*) malloc(sizeof(SSElement));
    if (element == NULL)
        ERROR("Allocation failed!");

    ScopeList *data = ScopeList_Init();
    // to send the problem further
    if (data == NULL)
        return NULL;

    element->data = data;
    element->next = next;

    return element;
}

void SSElement_Destroy(SSElement *element)
{
    if (element != NULL)
    {
        ScopeList_Destroy(element->data);
        free(element);
    }
}

ScopeStack *ScopeStack_Init()
{
    ScopeStack *stack = (ScopeStack*) malloc(sizeof(ScopeStack));
    if (stack == NULL)
        ERROR("Allocation failed!");

    stack->top = NULL;

    return stack;
}

void ScopeStack_Destroy(ScopeStack *stack)
{
    if (stack != NULL)
        free(stack);
}

ScopeList *ScopeStack_Push(ScopeStack *stack)
{
    if (stack == NULL)
        ERROR("Invalid parameter");

    SSElement *element = SSElement_Init(stack->top);
    if (element == NULL)
        return NULL;

    stack->top = element;
    
    return element->data;
}

ScopeList *ScopeList_Top(ScopeStack *stack)
{
    if (stack == NULL)
        ERROR("Invalid parameter!");

    if (stack->top == NULL)
        ERROR("Stack is empty, can't access top element!");

    return stack->top->data;
}

ScopeList *ScopeStack_Pop(ScopeStack *stack)
{
    if (stack == NULL)
        ERROR("Invalid parameter!");

    if (stack->top == NULL)
    {
        WARNING("Stack is empty, nothing to pop!");
        return NULL;
    }

    ScopeList *to_return = stack->top->data;
    SSElement *to_delete = stack->top;
    stack->top = to_delete->next;
    
    SSElement_Destroy(to_delete);

    return to_return;
}