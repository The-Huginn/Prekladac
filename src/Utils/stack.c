// IFJ Project 2021
/**
 * @file stack.c
 * @brief This file contains implementetaion stack.h
 * @author Denis Pojezdal (xpojez00)
 */
#include"stack.h"
#include"logger.h"

#include<stdlib.h>

typedef struct StackElement
{
	void* data;
	struct StackElement* next;

} SElement;

SElement* SElement_Init(void* data, SElement* next)
{
	SElement* sElement = (SElement*) malloc(sizeof(SElement));
	if (sElement == NULL)
		ERROR("Allocation failed!");

	if (data == NULL)
		WARNING("Data points to NULL!");

	sElement->data = data;
	sElement->next = next;

	return sElement;
}

void SElement_Destroy(SElement* sElement, void (*DataDtor)(void*))
{
	if (sElement == NULL)
		ERROR_VOID("Invalid argument!");

	if (DataDtor != NULL)
		DataDtor(sElement->data);

	free(sElement);
}

Stack* Stack_Init(void (*DataDtor)(void*))
{
	if (DataDtor == NULL)
		WARNING("Missing DataDtor!");
		
	Stack* stack = (Stack*) malloc(sizeof(Stack)); // not sure this works
	if (stack == NULL)
		ERROR("Allocation failed!");

	stack->DataDtor = DataDtor;
	stack->top = NULL;

	return stack;
}

void Stack_Destroy(Stack *stack)
{
	if (stack == NULL)
		ERROR_VOID("Stack does not exist!");
		
	Stack_Clear(stack);
	free(stack);
}

void* Stack_Push(Stack* stack, void* data)
{
	if (stack == NULL || data == NULL)
		ERROR("Invalid argument!");

	SElement* newTop = SElement_Init(data, stack->top);
	if (newTop == NULL)
		ERROR("Allocation failed!");

	stack->top = newTop;
	return data;
}

void Stack_Pop(Stack* stack)
{
	if (stack == NULL)
		ERROR_VOID("Invalid argument!");
	if (stack->top == NULL)
		ERROR_VOID("Stack is empty!");

	SElement* temp = stack->top->next;

	SElement_Destroy(stack->top, stack->DataDtor);
	stack->top = temp;
}

void* Stack_Top(Stack* stack)
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

bool Stack_IsEmpty(Stack* stack)
{
	if (stack == NULL)
		ERROR("Invalid argument!");

	return (stack->top == NULL) ? true : false;
}
