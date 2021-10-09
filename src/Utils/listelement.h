#ifndef __LIST_ELEMENT__
#define __LIST_ELEMENT__

#include "dlist.h"

typedef struct ListElement
{
	char* name;
	DLList dlist; // maybe pointer

	struct ListElement* next;
} LElement;

#endif // !__LIST_ELEMENT__
