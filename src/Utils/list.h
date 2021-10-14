/**
 * @file list.h
 * @brief This file contains Interface for working with list
 * @author Rastislav Budinsky
 */
#ifndef __LIST__
#define __LIST__

#include "stack.h"
#include <stdbool.h>

struct ListElement;

typedef struct LinkedList
{
	struct ListElement *begin;
	struct ListElement* active;
	void (*DataDtor)(void*);
	bool (*Comp)(void*, void*);
} LList;

/**
 * @brief creates and initializes list
 * @param DataDtor Destructor of the list elements
 * @param Comp Comparator for conditional search
 * @return pointer to the new list, might return NULL
 */
LList *List_Init(void (*DataDtor)(void*), bool (*Comp)(void*, void*));

/**
 * @brief destruction function, to clear whole list and should not be used afterwards
 * @param list struct to be deleted
 */
void List_Destroy(LList *list);

/**
 * @brief clears all elements of the list
 * @param list list to be cleared
*/
void List_Clear(LList* list);

/**
 * @brief Adds new element at the begining of the list
 * @param list List 
 * @param data Data of the new element
 * @return Pointer to created element
*/
void* List_AddFirst(LList* list, void* data);

/**
 * @brief Removes first element of the list
 * @param list List
*/
void List_RemoveFirst(LList* list);

/**
 * @brief Finds first element for which Comp returns true
 * @param list struct where to find the element
 * @param con Data to compare elements to
 * @return Pointer to first match
 */
void *List_GetData(LList *list, void* con);

/**
 * @param list struct to be checked
 * @return true if list is empty otherwise false
 */
bool List_IsEmpty(LList *list);

/**
 * @brief returns the first element of the list
 * @param list the list to get the first element of
 * @return pointer to this element, if empty, then NULL
 */
void *List_GetFirst(LList *list);

void List_SetFirstActive(LList* list);

void List_SetNextActive(LList* list);

void* List_GetActive(LList* list);

#endif // !__LIST__
