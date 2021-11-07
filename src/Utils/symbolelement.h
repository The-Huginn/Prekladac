/**
 * @file symbolelement.h
 * @brief This file contains struct for element from Symbol Table and
 * 	interface for working with this struct
 * @authors Denis Pojezdal, Rastislav Budinsky
 */
#ifndef __SYMBOL_ELEMENT__
#define __SYMBOL_ELEMENT__

#include <stdbool.h>

typedef enum SymbolType
{
	VARIABLE, FUNCTION // probably uncomplete

} SymbolType;

typedef struct SymbolElement Element;

/**
 * @brief Allocates memory and initializes it according to arguments
 * @param key Pointer to already allocated string
 * @param type 
 * @param isDefined 
 * @param data 
 * @return Pointer to the Element
*/
Element* Element_Init(const char* key, SymbolType type, bool isDefined, void* data);

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
 * @brief
 * @param element Element
 * @return Pointer to data
 */
void *Element_GetData(Element *element);

/**
 * @brief Checks, whether 2 elements pointers are the same
 * @param element1 The first element
 * @param element2 The second element
 * @return True is both pointers are the same otherwise false
 */
bool Element_IsEqual(Element *element1, Element *element2);

#endif // !__SYMBOL_ELEMENT__
