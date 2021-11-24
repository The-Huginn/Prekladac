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

Element* Element_Init(const char* key, SymbolType type, int id)
{
	Element *element = (Element*) malloc(sizeof(Element));
	if (element == NULL)
		ERROR("Allocation failed!");

	element->key = key;
	element->type = type;
	element->isDefined = false;
	element->ID = id;
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

bool Element_IsVariable(Element *element)
{
	return element->type == VARIABLE;
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

	ERROR_VOID("Called with invalid element");
}

void Element_AddReturn(Element *element, SemanticType type)
{
	if (element->type != FUNCTION)
		ERROR_VOID("Called with invalid element");

	// should change only upon first set Semantic type
	if (element->semanticType == SEMANTIC_VOID)
		element->semanticType = type;

	FunctionData_AddSemanticReturn(element->data, type);
}

void Element_AddParam(Element *element, SemanticType type, char* id)
{
	if (element->type == FUNCTION)
		FunctionData_AddSemanticParam(element->data, type, id);
	else
		WARNING("Function only function called upon Variable!");
}

SemanticType Element_GetSemantic(Element *element)
{
	return element->semanticType;
}

int Element_FunctionParameters_Size(Element *element)
{
	return element->type == FUNCTION ? FunctionData_Params_Size(element->data) : -1;
}

int Element_FunctionReturns_Size(Element *element)
{
	return element->type == FUNCTION ? FunctionData_Returns_Size(element->data) : -1;
}

const char *Element_FunctionParameter_GetName(Element *element, int index)
{
	return FunctionData_Params_GetName(element->data, index);
}

void Element_FunctionParameter_SetName(Element *element, int index, const char* id)
{
	FunctionData_Params_SetName(element->data, index, id);
}

void Element_FunctionParameter_SetSemantic(Element *element, int index, SemanticType semanticType)
{
	FunctionData_Params_SetSemantic(element->data, index, semanticType);
}

SemanticType Element_FunctionParameter_GetSemantic(Element *element, int index)
{
	if (element->type != VARIABLE)
	{
		WARNING("Function only function called upon Variable!");
		return SEMANTIC_VOID;
	}
	return FunctionData_Params_GetSemantic(element->data, index);
}

SemanticType Element_FunctionReturn_GetSemantic(Element *element, int index)
{
	if (element->type != VARIABLE)
	{
		WARNING("Function only function called upon Variable!");
		return SEMANTIC_VOID;
	}
	return FunctionData_Returns_GetSemantic(element->data, index);
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