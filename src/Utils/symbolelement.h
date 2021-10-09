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

	struct SymbolElement* next;
	struct SymbolElement* prev;

} Element;

#endif // !__SYMBOL_ELEMENT__
