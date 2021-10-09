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
	char* name;
	void* data;

	struct SymbolElement* next;
	struct SymbolElement* prev;

} SElement;

#endif // !__SYMBOL_ELEMENT__
