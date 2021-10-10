#include "symbolelement.h"

#include <stdlib.h>

Element* Element_Init(char* key, SymbolType type, int isDefined, void* data)
{
	return NULL;
}

void Element_Free(Element* element)
{
	// TO DO free shit saved in structure pointed to by data
	free(element->data);
	// TO DO maybe free the key value, but it might be shared amongs all elements of stack
	return;
}
