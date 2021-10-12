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
	SymbolType type;	//! type of Symbol Element for further casting and destructors
	bool isDefined;		//! bool value deciding whether Symbol was defined
	const char* key;	//! pointer to the string with key, currently is not allocated in SymbolElement
	void* data;			//! pointer to the data, should be casted

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
	// TO DO free shit saved in structure pointed to by data
	free(element->data);
	// TO DO maybe free the key value, but it might be shared amongs all elements of stack
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
	return element1->data == element2->data;
}