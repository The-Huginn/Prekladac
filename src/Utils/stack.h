#ifndef __STACK__
#define __STACK__

#include "symbolelement.h"

typedef struct StackElement
{
	Element* data;
	struct StackElement* next;

} SElement;

/**
 * @brief Initializes StackElement
 * @param data StackElement data 
 * @param next Next element
 * @return 
*/
SElement* SElement_Init(Element* data, SElement* next);

/**
 * @brief Frees all allocated memory
 * @param sElement Element to be freed
*/
void SElement_Free(SElement* sElement);

typedef struct Stack
{
	SElement* top;
} Stack;

/**
 * @brief Inits stack to NULL
 * @return Initialized stack
*/
Stack* Stack_Init();

/**
 * @brief Adds element at the stack top
 * @param stack Stack to be expanded
 * @param element Element to be added
 * @return Pointer to added element
*/
Element* Stack_Push(Stack* stack, Element* element);

/**
 * @brief Removes top element from stack
 * @param stack Stack to be removed from
 * @return 0 if failed
*/
int Stack_Pop(Stack* stack);

/**
 * @brief Access top element 
 * @param stack Stack to be searched
 * @return Top element
*/
Element* Stack_Top(Stack* stack);

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
int Stack_IsEmpty(Stack* stack);


#endif // !__STACK__
