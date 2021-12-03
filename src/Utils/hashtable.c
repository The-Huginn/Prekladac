// IFJ Project 2021
#include<string.h>
#include <stdbool.h>
#include"hashtable.h"
#include"logger.h"

bool DuplicateList_Comp(Jesus* jesus, char* key)
{
	if (jesus == NULL)
		ERROR("Invalid argument!");

	if (Jesus_GetKey(jesus) == NULL)
		return false;

	return strcmp(Jesus_GetKey(jesus), key) == 0;
}

HTable* HashTable_Init(const int32_t size)
{
	HTable* table = (HTable*)malloc(sizeof(HTable));
	if (table == NULL)
		ERROR("Allocation failed!");

	table->array = malloc(size * sizeof(LList*));
	if (table->array == NULL)
	{
		free(table);
		ERROR("Allocation failed!");
	}

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

Jesus* HashTable_Find(HTable* table, const char* key)
{
	if (table == NULL || key == NULL)
		ERROR("Invalid argument!");

	uint32_t hash = HashTable_Hash(key) % table->size;

	if (table->array[hash] == NULL)
		return NULL;
	
	return List_GetData(table->array[hash], key);
}

Jesus* HashTable_Insert(HTable* table, const char* key)
{
	if (table == NULL || key == NULL)
		ERROR("Invalid argument!");

	uint32_t hash = HashTable_Hash(key) % table->size;

	if (table->array[hash] == NULL)
	{
		table->array[hash] = List_Init((void ( * )(void*)) Jesus_Destroy, (const bool ( * )(void*, const void*)) DuplicateList_Comp);

		Stack *stack = Stack_Init((void ( * ) (void*)) NULL);  // clearing of stack is done in custom way
        if (stack == NULL)
            return NULL;

		Jesus *jesus = Jesus_Init(key, stack);

		List_AddFirst(table->array[hash], jesus);
	}

	return List_GetData(table->array[hash], key);
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

	free(table->array);
	free(table);

	return;
}
