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
 * @brief Hashes string key to 32bit value
 * @param key String to be hashed
 * @return Hash value of string
*/
uint32_t HashTable_Hash(const char* key); // declaration might be removed

/**
 * @brief Finds data coresponding to a key
 * @param table Hash table to be searched
 * @param key Hash key
 * @return Pointer to list with given hash value
*/
Jesus* HashTable_Find(HTable* table, const char* key);

/**
 * @brief Adds new element with given key
 * @param table Hash table to be updated
 * @param key Hash key
 * @return Pointer to newly created list, or to already existing one
*/
Jesus* HashTable_Insert(HTable* table, const char* key);

/**
 * @brief Removes element with given key
 * @param table Hash table to be removed from
 * @param key Hash key
 * @return Non zero value if succeeded
*/
int HashTable_Remove(HTable* table, const char* key);

/**
 * @brief Frees allocated memory of hash table
 * @param table Hash table to be freed
*/
void HashTable_Destroy(HTable* table);

#endif // !__HASH_TABLE__
