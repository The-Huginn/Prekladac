#ifndef __HASH_TABLE__
#define __HASH_TABLE__

#include <stdint.h>
#include "list.h"
#include "jesus.h"

typedef struct HashTable
{
	uint32_t size;

	LList** array;
} HTable;

/**
 * @brief Initializes array in hash table
 * @param size Size of array to be allocated
 * @return Initialized hash table
*/
HTable* HashTable_Init(const int32_t size);

/**
 * @brief Finds data coresponding to a key
 * @param table Hash table
 * @param key String key
 * @return Pointer to struct Jesus<string, stack>
*/
Jesus* HashTable_Find(HTable* table, const char* key);

/**
 * @brief Adds new element with given key
 * @param table Hash table
 * @param key String key
 * @return Pointer to newly created struct Jesus<string, stack>
*/
Jesus* HashTable_Insert(HTable* table, const char* key);

/**
 * @brief Removes element with given key
 * @param table Hash table
 * @param key String key
 * @return Non zero value if succeeded
*/
int HashTable_Remove(HTable* table, const char* key);

/**
 * @brief Frees allocated memory of hash table
 * @param table Hash table
*/
void HashTable_Destroy(HTable* table);

#endif // !__HASH_TABLE__
