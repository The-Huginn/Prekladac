/**
 * @file symbolelement.h
 * @brief This file contains struct for element from Symbol Table and
 * 	interface for working with this struct
 * @authors Denis Pojezdal, Rastislav Budinsky
 */
#ifndef __SYMBOL_ELEMENT__
#define __SYMBOL_ELEMENT__

#include "semantictype.h"
#include "vector.h"

#include <stdbool.h>

typedef enum SymbolType
{
	VARIABLE, FUNCTION // probably uncomplete

} SymbolType;

typedef struct SymbolElement Element;

/**
 * @brief Allocates memory and initializes it according to arguments
 * @param key Pointer to already allocated string
 * @param type SymbolType
 * @return Pointer to the Element
*/
Element* Element_Init(const char* key, SymbolType type);

/**
 * @brief Frees all allocated memory
 * @param element Element to be freed
*/
void Element_Destroy(Element* element);

/**
 * @brief Returns pointer to allocated string by the struct disctinguishing identifiers
 * @param element Element
 * @return Pointer to the first char
 */
const char *Element_GetKey(Element *element);

/**
 * @brief
 * @param element Element
 * @return True is element was previously defined otherwise false
 */
bool Element_IsDefined(Element *element);

/**
 * @brief Set defined to 1 regardless of previous state
 * @param element Element
 */
void Element_Define(Element *element);

/**
 * @param element Element
 * @param new_type If element is Variable then changes Semantic type of variable, if function then adds new return value Semantic type
 */
void Element_SetSemantic(Element *element, SemanticType new_type);

/**
 * @brief Should be called only upon Function
 * @param element Element
 * @param type Semantic type of newly added parameter @note this adds new parameter and sets it's Semantic type
 */
void Element_AddSemanticParam(Element *element, SemanticType type);

/**
 * @param element Element
 * @return Semantic type of Element, if function then Semantic type of first return value 
 */
SemanticType Element_GetSemantic(Element *element);

/**
 * @brief Should be called only upon Function
 * @param element Element
 * @return Vector* of SemanticType* of parameters
 */
Vector *Element_GetFunctionParameters(Element *element);

/**
 * @brief Should be called only upon Function
 * @param element Element
 * @return Vector* of SemanticType* of return values
 */
Vector *Element_GetFunctionReturns(Element *element);

/**
 * @brief
 * @param element Element
 * @param new_ID new ID to assign
 */
void Element_SetID(Element *element, int new_ID);

/**
 * @param element Element
 * @return ID previously assigned to element otherwise -1
 */
int Element_GetID(Element *element);

/**
 * @brief Checks, whether 2 elements pointers are the same
 * @param element1 The first element
 * @param element2 The second element
 * @return True is both pointers are the same otherwise false
 */
bool Element_IsEqual(Element *element1, Element *element2);

#endif // !__SYMBOL_ELEMENT__
