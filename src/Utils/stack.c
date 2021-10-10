/**
 * @file stack.c
 * @brief This file contains implementetaion stack.h
 * @author Denis Pojezdal
 */
#include"stack.h"
#include"logger.h"

#include<stdlib.h>

typedef struct StackElement
{
	Element* data;
	struct StackElement* next;

} SElement;

SElement* SElement_Init(Element* data, SElement* next)
{
	SElement* sElement = (SElement*) malloc(sizeof(SElement));
	if (sElement == NULL)
		ERROR("Allocation failed!");

	sElement->data = data;
	sElement->next = next;

	return sElement;
}

void SElement_Free(SElement* sElement)
{
	Element_Free(sElement->data);
	free(sElement);
}

Stack* Stack_Init()
{
	Stack* stack = (Stack*) malloc(sizeof(Stack)); // not sure this works
	if (stack == NULL)
		ERROR("Allocation failed!");
	return stack;
}

void Stack_Destroy(Stack *stack)
{
	if (stack == NULL)
		return;
		
	Stack_Clear(stack);
	free(stack);
}

Element* Stack_Push(Stack* stack, Element* element)
{
	if (stack == NULL || element == NULL)
		ERROR("Invalid argument!");

	SElement* newTop = SElement_Init(element, stack->top);
	if (newTop == NULL)
		ERROR("Allocation failed!");

	stack->top = newTop;
	return element;
}

int Stack_Pop(Stack* stack)
{
	if (stack == NULL)
		ERROR("Invalid argument!");
	if (stack->top == NULL)
		ERROR("Stack is empty!");

	SElement* temp = stack->top->next;
	SElement_Free(stack->top);
	stack->top = temp;
	return 1;
}

Element* Stack_Top(Stack* stack)
{
	if (stack == NULL)
		ERROR("Invalid argument!");
	if (stack->top == NULL)
		ERROR("Stack is empty!");
	
	return stack->top->data;
}

void Stack_Clear(Stack* stack)
{
	if (stack == NULL)
		ERROR_VOID("Invalid argument!");

	while (stack->top != NULL)
		Stack_Pop(stack);

	return;
}

int Stack_IsEmpty(Stack* stack)
{
	if (stack == NULL)
		ERROR("Invalid argument!");

	return (stack->top == NULL) ? 1 : 0;
}
