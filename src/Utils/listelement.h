#ifndef __LIST_ELEMENT__
#define __LIST_ELEMENT__

#include "stack.h"

typedef struct ListElement
{
	char* key;
	Stack dlist; // maybe pointer

	struct ListElement* next;
} LElement;

#endif // !__LIST_ELEMENT__
