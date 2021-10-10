/**
 * @file list.h
 * @brief This file contains Interface for working with list
 * @author Rastislav Budinsky
 */
#ifndef __LIST__
#define __LIST__

#include "stack.h"

struct ListElement;

typedef struct LinkedList
{
	struct ListElement *begin;
} LList;

/**
 * @brief creates and initializes list
 * @return pointer to the new list, might return NULL
 */
LList *List_Init();

/**
 * @brief destruction function, to clear whole list and should not be used afterwards
 * @param list struct to be deleted
 */
void List_Destroy(LList *list);

/**
 * @brief finds the key in the list otherwise it creates new one
 * @param list struct where to find the stack in
 * @param key string to find a match in the list
 * @return pointer to the existing or newly created stack
 */
Stack *List_GetStack(LList *list, const char* key);

#endif // !__LIST__
