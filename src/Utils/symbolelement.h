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
 * @brief Allocates memory and initializes it according to arguments @note This function should be called only via Symtable interface
 * @param key Pointer to already allocated string
 * @param type SymbolType
 * @param id Unique ID based on scope
 * @return Pointer to the Element
*/
Element* Element_Init(const char* key, SymbolType type, int id);

/**
 * @brief Frees all allocated memory @note This function should be called only via Symtable interface
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
 * @return True if element was previously defined otherwise false
 */
bool Element_IsDefined(Element *element);

/**
 * @brief Set defined to true regardless of previous state
 * @param element Element
 */
void Element_Define(Element *element);

/**
 * @brief Should be called only upon Variable
 * @param element Element
 * @param new_type Semantic type of Variable
 */
void Element_SetSemantic(Element *element, SemanticType new_type);

/**
 * @brief Should be called only upon Function
 * @param element Element
 * @param type Semantic type of newly added parameter @note this adds new parameter and sets it's Semantic type
 */
void Element_AddReturn(Element *element, SemanticType type);

/**
 * @brief Should be called only upon Function
 * @param element Element
 * @param type Semantic type of newly added parameter @note this adds new parameter and sets it's Semantic type
 * @param id pointer to the name of id @note if name not know set to NULL
 */
void Element_AddParam(Element *element, SemanticType type, char* id);

/**
 * @param element Element
 * @return Semantic type of Element, if function then Semantic type of first return value 
 */
SemanticType Element_GetSemantic(Element *element);

/**
 * @param element Element
 * @return Count of function parameters
 */
int Element_FunctionParameters_Size(Element *element);

/**
 * @param element Element
 * @return Count of function returns
 */
int Element_FunctionReturns_Size(Element *element);

/**
 * @brief Should be called only upon Function
 * @param element Element
 * @param index Index of parameter
 * @return char * of name @note do not change content
 */
const char *Element_FunctionParameter_GetName(Element *element, int index);

/**
 * @brief Should be called only upon Function
 * @param element Element
 * @param index Index of parameter
 * @param id New name for the parameter
 * @return Vector* of SemanticType* of parameters
 */
void Element_FunctionParameter_SetName(Element *element, int index, const char* id);

/**
 * @brief Should be called only upon Function
 * @param element Element
 * @param index Index of parameter
 * @return Semantic type of parameter
 */
SemanticType Element_FunctionParameter_GetSemantic(Element *element, int index);

/**
 * @brief Should be called only upon Function
 * @param element Element
 * @param index Index of return value
 * @return Semantic type of indexed return value
 */
SemanticType Element_GetFunctionReturn_Semantic(Element *element, int index);

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
