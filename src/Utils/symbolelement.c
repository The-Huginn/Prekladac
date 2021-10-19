/**
 * @file symbolelement.c
 * @brief This file implements whole interface of symbolelement.h
 * @authors Denis Pojezdal, Rastislav Budinsky
 */
#include "symbolelement.h"
#include "logger.h"

#include <stdlib.h>

struct SymbolElement
{
	SymbolType type;	//! Type of Symbol Element for further casting and destructors
	bool isDefined;		//! Bool value deciding whether Symbol was defined
	const char* key;	//! Pointer to the string with key, currently is not allocated in SymbolElement
	void* data;			//! Pointer to the data, should be casted

};

Element* Element_Init(const char* key, SymbolType type, bool isDefined, void* data)
{
	Element *element = (Element*) malloc(sizeof(Element));
	if (element == NULL)
		ERROR("Allocation failed!");

	element->key = key;
	element->type = type;
	element->isDefined = isDefined;
	element->data = data;
	
	return element;
}

void Element_Destroy(Element* element)
{
	if (element->data != NULL)
		free(element->data);

	free(element);
	return;
}


const char *Element_GetKey(Element *element)
{
	return element->key;
}

bool Element_IsDefined(Element *element)
{
	return element->isDefined;
}

void Element_Define(Element *element)
{
	element->isDefined = 1;
}

void *Element_GetData(Element *element)
{
	return element->data;
}

bool Element_IsEqual(Element *element1, Element *element2)
{
	return element1 == element2;
}