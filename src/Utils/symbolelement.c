/**
 * @file symbolelement.c
 * @brief This file implements whole interface of symbolelement.h
 * @authors Denis Pojezdal, Rastislav Budinsky
 */
#include "symbolelement.h"
#include "logger.h"
#include "functiondata.h"

#include <stdlib.h>

struct SymbolElement
{
	SymbolType type;	//! Type of Symbol Element for further casting and destructors
	bool isDefined;		//! Bool value deciding whether Symbol was defined
	const char* key;	//! Pointer to the string with key, currently is not allocated in SymbolElement
	void* data;			//! Pointer to the data, should be casted
	int ID;				//! ID of current variable instance
	SemanticType semanticType;	//! Semantic Type of Variable of first return value
};

Element* Element_Init(const char* key, SymbolType type)
{
	Element *element = (Element*) malloc(sizeof(Element));
	if (element == NULL)
		ERROR("Allocation failed!");

	element->key = key;
	element->type = type;
	element->isDefined = false;
	element->ID = -1;
	element->semanticType = SEMANTIC_VOID;
	
	if (type == FUNCTION)
	{
		element->data = FunctionData_Init(type);
		if (element->data == NULL)
		{
			free(element);
			return NULL;
		}
	}
	
	return element;
}

void Element_Destroy(Element* element)
{
	if (element->type == FUNCTION)
		FunctionData_Destroy(element->data);

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
	if (new_type == SEMANTIC_VOID)
		WARNING("Setting element to VOID");

	// return if variable
	if (element->type == VARIABLE)
	{
		element->semanticType = new_type;
		return;
	}

	// should change only upon first set Semantic type
	if (element->type == FUNCTION && element->semanticType == SEMANTIC_VOID)
		element->type = new_type;

	FunctionData_AddSemanticReturn(element->data, new_type);
}

void Element_AddSemanticParam(Element *element, SemanticType type)
{
	if (element->type == FUNCTION)
		FunctionData_AddSemanticParam(element->data, type);
	else
		WARNING("Function only function called upon Variable!");
}

SemanticType Element_GetSemantic(Element *element)
{
	return element->semanticType;
}

Vector *Element_GetFunctionParameters(Element *element)
{
	if (element->type == VARIABLE)
	{
		WARNING("Function only function called upon Variable!");
		return NULL;
	}
	return FunctionData_Params(element->data);
}

Vector *Element_GetFunctionReturns(Element *element)
{
	if (element->type == VARIABLE)
	{
		WARNING("Function only function called upon Variable!");
		return NULL;
	}
	return FunctionData_ReturnVals(element->data);
}

void Element_SetID(Element *element, int new_ID)
{
	element->ID = new_ID;
}

int Element_GetID(Element *element)
{
	return element->ID;
}

bool Element_IsEqual(Element *element1, Element *element2)
{
	return element1 == element2;
}