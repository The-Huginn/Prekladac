#include<string.h>
#include"hashtable.h"
#include"logger.h"

HTable* HashTable_Init(const int32_t size)
{
	HTable* table = malloc(sizeof(uint32_t) + size * sizeof(LList*));
	if (table == NULL)
		ERROR("Allocation failed!");

	table->size = size;
	memset(table->array, 0, size * sizeof(LList*));
	return table;
}

uint32_t HashTable_Hash(const char* key)
{
	uint32_t hash = 5381;
	const unsigned char* p;
	for (p = (const unsigned char*)key; *p != '\0'; p++)
		hash = ((hash << 5) + hash) + *p;

	return hash;
}

LList* HashTable_Find(HTable* table, const char* key)
{
	if (table == NULL)
		ERROR("Invalid argument!");

	uint32_t hash = HashTable_Hash(key) % table->size;
	LList* list = table->array[hash];
	while (list != NULL)
	{
		//if (strcmp(list->data, key) == 0)
		//	return &(data->data);
		//data = data->next;
	}
	return NULL;
}
