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

	DataDtor(sElement->data);
	free(sElement);
}

Stack* Stack_Init(void (*DataDtor)(void*))
{
	if (DataDtor == NULL)
		ERROR("Invalid parameter!");
		
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

void* Stack_Push(Stack* stack, void* element)
{
	if (stack == NULL || element == NULL)
		ERROR("Invalid argument!");

	SElement* newTop = SElement_Init(element, stack->top);
	if (newTop == NULL)
		ERROR("Allocation failed!");

	stack->top = newTop;
	return element;
}

void* Stack_Pop(Stack* stack)
{
	if (stack == NULL)
		ERROR("Invalid argument!");
	if (stack->top == NULL)
		ERROR("Stack is empty!");

	SElement* temp = stack->top->next;
	void* ret = stack->top->data;

	SElement_Destroy(stack->top, stack->DataDtor);
	stack->top = temp;
	return ret;
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

int Stack_IsEmpty(Stack* stack)
{
	if (stack == NULL)
		ERROR("Invalid argument!");

	return (stack->top == NULL) ? 1 : 0;
}
