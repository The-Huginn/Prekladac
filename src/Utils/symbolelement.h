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
	VARIABLE, FUNCTION // probably uncompleate

} SymbolType;

typedef struct SymbolElement Element;

/**
 * @brief Allocates memory and initializes it according to arguments
 * @param key 
 * @param type 
 * @param isDefined 
 * @param data 
 * @return 
*/
Element* Element_Init(const char* key, SymbolType type, bool isDefined, void* data);

/**
 * @brief Frees all allocated memory
 * @param element Element to be freed
*/
void Element_Destroy(Element* element);

/**
 * @brief returns pointer allocated string by the struct disctinguishing identifiers
 * @param element the Element we want to get info about
 * @return Pointer to the first char
 */
const char *Element_GetKey(Element *element);

/**
 * @brief
 * @param element the Element we want to get info about
 * @return returns 1 upon defined Symbol Element, otherwise 0
 */
bool Element_IsDefined(Element *element);

/**
 * @brief Set define to 1 regardless of previous state
 * @param element the Element we want to change info of
 */
void Element_Define(Element *element);

/**
 * @brief
 * @param element the Element we want to get info about
 * @return the pointer to the stored data
 */
void *Element_GetData(Element *element);

/**
 * @brief checks, whether 2 elements pointers are the same
 * @param element1 the first element
 * @param element2 the second element
 * @return if both elements pointers are the same
 */
bool Element_IsEqual(Element *element1, Element *element2);

#endif // !__SYMBOL_ELEMENT__
