#ifndef __SYMBOL_ELEMENT__
#define __SYMBOL_ELEMENT__

typedef enum SymbolType
{
	VARIABLE, FUNCTION // probably uncompleate

} SymbolType;

typedef struct SymbolElement
{
	SymbolType type;
	int isDefined;
	char* key;
	void* data;

} Element;

/**
 * @brief 
 * @param key 
 * @param type 
 * @param isDefined 
 * @param data 
 * @return 
*/
Element* Element_Init(char* key, SymbolType type, int isDefined, void* data);

/**
 * @brief Frees all allocated memory
 * @param element Element to be freed
*/
void Element_Free(Element* element);



#endif // !__SYMBOL_ELEMENT__
