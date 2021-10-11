/**
 * @file scope_stack.h
 * @brief This file contains interface for ScopeStack
 * @author Rastislav Budinsky
 */
#ifndef __SCOPE_STACK_H__
#define __SCOPE_STACK_H__

#include <stdlib.h>
#include "scope_list.h"

struct SStackElement;

typedef struct 
{
    struct SStackElement *top;
} ScopeStack;

/**
 * @brief creates and initializes struct ScopeStack
 * @return the pointer to the newly created ScopeStack
 */
ScopeStack *ScopeStack_Init();

/**
 * @brief destroys the struct, should not be used afterwards, NULL has no affect
 * @param stack ScopeStack to be destroyed
 */
void ScopeStack_Destroy(ScopeStack *stack);

/**
 * @brief adds new ScopeList element on top of stack
 * @param stack the stack to which the element is added
 * @return pointer to the newly created ScopeList
 */
ScopeList *ScopeStack_Push(ScopeStack *stack);

/**
 * @param stack the stack to get top element of
 * @return the pointer to the top ScopeList
 */
ScopeList *ScopeList_Top(ScopeStack *stack);

/**
 * @brief pops the top element from the stack
 * @param stack the stack to pop the top element
 * @return the top ScopeList element, NULL in problem cases
 */
ScopeList *ScopeStack_Pop(ScopeStack *stack);

#endif // !__SCOPE_STACK_H__