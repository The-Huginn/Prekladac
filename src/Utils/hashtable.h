#ifndef __HASH_TABLE__
#define __HASH_TABLE__

#include <stdint.h>
#include "list.h"

typedef struct HashTable
{
	uint32_t size;

	LList* array[];
} HTable;

#endif // !__HASH_TABLE__
