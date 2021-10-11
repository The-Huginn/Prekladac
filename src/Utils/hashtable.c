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
	if (table == NULL || key == NULL)
		ERROR("Invalid argument!");

	uint32_t hash = HashTable_Hash(key) % table->size;
	
	return table->array[hash];
}

LList* HashTable_Insert(HTable* table, const char* key)
{
	if (table == NULL || key == NULL)
		ERROR("Invalid argument!");

	uint32_t hash = HashTable_Hash(key) % table->size;

	if (table->array[hash] == NULL)
		table->array[hash] = List_Init();

	return table->array[hash];
}

int HashTable_Remove(HTable* table, const char* key)
{
	if (table == NULL || key == NULL)
		ERROR("Invalid argument!");

	uint32_t hash = HashTable_Hash(key) % table->size;

	if (table->array[hash] == NULL)
		ERROR("List with given hash does not exists!");

	List_Destroy(table->array[hash]);
	return 1;
}

void HashTable_Destroy(HTable* table)
{
	if (table == NULL)
		ERROR_VOID("Invalid argument!");

	for (int i = 0; i < table->size; i++)
		if (table->array[i] != NULL)
			List_Destroy(table->array[i]);

	free(table); // not sure this works 

	return;
}
