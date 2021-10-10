/**
 * @file symbolelement.h
 * @brief This file contains struct for element from Symbol Table and
 * 	interface for working with this struct
 * @authors Denis Pojezdal, Rastislav Budinsky
 */
#ifndef __SYMBOL_ELEMENT__
#define __SYMBOL_ELEMENT__

typedef enum SymbolType
{
	VARIABLE, FUNCTION // probably uncompleate

} SymbolType;

typedef struct SymbolElement
{
	SymbolType type;	//! type of Symbol Element for further casting and destructors
	char isDefined;		//! bool value deciding whether Symbol was defined
	char* key;			//! pointer to the string with key, currently is not allocated in SymbolElement
	void* data;			//! pointer to the data, should be casted

} Element;

/**
 * @brief Allocates memory and initializes it according to arguments
 * @param key 
 * @param type 
 * @param isDefined 
 * @param data 
 * @return 
*/
Element* Element_Init(char* key, SymbolType type, char isDefined, void* data);

/**
 * @brief Frees all allocated memory
 * @param element Element to be freed
*/
void Element_Free(Element* element);

/**
 * @brief returns pointer allocated string by the struct disctinguishing identifiers
 * @param element the Element we want to get info about
 * @return Pointer to the first char
 */
char *Element_GetKey(Element *element);

/**
 * @brief
 * @param element the Element we want to get info about
 * @return returns 1 upon defined Symbol Element, otherwise 0
 */
char Element_IsDefined(Element *element);

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

#endif // !__SYMBOL_ELEMENT__
