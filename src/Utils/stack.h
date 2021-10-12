/**
 * @file stack.h
 * @brief This file defines struct for Stack and interface for working with it.
 * 	Stack was implemented via list structure
 * @author Denis Pojezdal
 */
#ifndef __STACK__
#define __STACK__

#include <stdbool.h>
#include "symbolelement.h"

struct StackElement;

typedef struct Stack
{
	struct StackElement* top;
	void (*DataDtor)(void*);
} Stack;

/**
 * @brief Inits stack empty stack
 * @param DataDtor Destructor function for data
 * @return Initialized stack
*/
Stack* Stack_Init(void (*DataDtor)(void*));

/**
 * @brief Destroys the stack, should not be used afterwards. NULL is ignored
 * @param stack the stack to be destroyed
 */
void Stack_Destroy(Stack *stack);

/**
 * @brief Adds element at the stack top
 * @param stack Stack to be expanded
 * @param element Element to be added
 * @return Pointer to added element
*/
void* Stack_Push(Stack* stack, void* element);

/**
 * @brief Removes top element from stack
 * @param stack Stack to be removed from
 * @return 0 if failed
*/
void* Stack_Pop(Stack* stack);

/**
 * @brief Access top element 
 * @param stack Stack to be searched
 * @return Top element
*/
void* Stack_Top(Stack* stack);

/**
 * @brief Clears all element from the stack
 * @param stack Stack to be cleared
*/
void Stack_Clear(Stack* stack);

/**
 * @brief Checkes if stack is empty
 * @param stack Stack to be checked
 * @return 1 if empty 0 if not
*/
bool Stack_IsEmpty(Stack* stack);


#endif // !__STACK__
