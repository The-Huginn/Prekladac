/**
 * @file symbolelement.c
 * @brief This file implements whole interface of symbolelement.h
 * @authors Denis Pojezdal, Rastislav Budinsky
 */
#include "symbolelement.h"
#include "logger.h"
#include "data.h"

#include <stdlib.h>

struct SymbolElement
{
	SymbolType type;	//! Type of Symbol Element for further casting and destructors
	bool isDefined;		//! Bool value deciding whether Symbol was defined
	const char* key;	//! Pointer to the string with key, currently is not allocated in SymbolElement
	void* data;			//! Pointer to the data, should be casted

};

Element* Element_Init(const char* key, SymbolType type)
{
	Element *element = (Element*) malloc(sizeof(Element));
	if (element == NULL)
		ERROR("Allocation failed!");

	element->key = key;
	element->type = type;
	element->isDefined = false;
	element->data = Data_Init(type);
	if (element->data == NULL)
	{
		free(element);
		return NULL;
	}
	
	return element;
}

void Element_Destroy(Element* element)
{
	if (element->type == FUNCTION)
		Data_Function_Destroy(element->data);
	else
		Data_Variable_Destroy(element->data);

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

void Element_SetSemantic(Element *element, SemanticType new_type)
{
	if (element->type == VARIABLE)
		Data_Variable_SetSemantic(element->data, new_type);
	else
		Data_AddSemanticReturn(element->data, new_type);
}

void Element_AddSemanticParam(Element *element, SemanticType type)
{
	Data_AddSemanticParam(element->data, type);
}

SemanticType Element_GetSemantic(Element *element)
{
	return Data_GetSemanticType(element->data, element->type);
}

Vector *Element_GetFunctionParameters(Element *element)
{
	return Data_Function_Params(element->data);
}

Vector *Element_GetFunctionReturns(Element *element)
{
	return Data_Function_ReturnVals(element->data);
}

void Element_SetID(Element *element, int new_ID)
{
	Data_SetID(element->data, element->type, new_ID);
}

int Element_GetID(Element *element)
{
	return Data_GetID(element->data, element->type);
}

bool Element_IsEqual(Element *element1, Element *element2)
{
	return element1 == element2;
}