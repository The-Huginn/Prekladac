/**
 * @file symbolelement.c
 * @brief This file implements whole interface of symbolelement.h
 * @authors Denis Pojezdal, Rastislav Budinsky
 */
#include "symbolelement.h"
#include "logger.h"

#include <stdlib.h>

Element* Element_Init(char* key, SymbolType type, char isDefined, void* data)
{
	Element *element = (Element*) malloc(sizeof(Element));
	if (element == NULL)
		ERROR("Allocation failed!\n");

	element->key = key;
	element->type = type;
	element->isDefined = isDefined;
	element->data = data;
	
	return element;
}

void Element_Free(Element* element)
{
	// TO DO free shit saved in structure pointed to by data
	free(element->data);
	// TO DO maybe free the key value, but it might be shared amongs all elements of stack
	return;
}


char *Element_GetKey(Element *element)
{
	return element->key;
}

char Element_IsDefined(Element *element)
{
	return element->isDefined;
}

void Element_Define(Element *element)
{
	element->isDefined = 1;
}

void *Element_GetData(Element *element)
{
	element->data;
}