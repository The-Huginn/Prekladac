/**
 * @file stack.h
 * @brief This file defines struct for Stack and interface for working with it.
 * 	Stack was implemented via list
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
 * @brief Inits empty stack
 * @param DataDtor Destructor function for data
 * @return Initialized stack
*/
Stack* Stack_Init(void (*DataDtor)(void*));

/**
 * @brief Destroys the stack, should not be used afterwards. NULL is ignored
 * @param stack Stack
 */
void Stack_Destroy(Stack *stack);

/**
 * @brief Adds element at the stack top
 * @param stack Stack
 * @param data Data to be added
 * @return Pointer to added data
*/
void* Stack_Push(Stack* stack, void* data);

/**
 * @brief Removes top element from stack
 * @param stack Stack
*/
void Stack_Pop(Stack* stack);

/**
 * @brief Access top element 
 * @param stack Stack
 * @return Top element's data
*/
void* Stack_Top(Stack* stack);

/**
 * @brief Clears all elements from the stack
 * @param stack Stack
*/
void Stack_Clear(Stack* stack);

/**
 * @brief Checks if stack is empty
 * @param stack Stack
 * @return True if is stack empty otherwise false
*/
bool Stack_IsEmpty(Stack* stack);


#endif // !__STACK__
