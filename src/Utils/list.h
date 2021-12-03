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
	const bool (*Comp)(void*, const void*);
} LList;

/**
 * @brief Creates and initializes list
 * @param DataDtor Destructor of the list element
 * @param Comp Comparator for conditional search
 * @return Pointer to the new list, might return NULL
 */
LList *List_Init(void (*DataDtor)(void*), const bool (*Comp)(void*, const void*));

/**
 * @brief Destruction function for destroying list with elements using their dtor
 * @param list struct to be deleted
 */
void List_Destroy(LList *list);

/**
 * @brief Clears all elements of the list using their dtor
 * @param list List
*/
void List_Clear(LList* list);

/**
 * @brief Adds new element at the beginning of the list
 * @param list List 
 * @param data Data of the new element
 * @return Pointer to created element's data
*/
void* List_AddFirst(LList* list, void* data);

/**
 * @brief Removes first element of the list
 * @param list List
*/
void List_RemoveFirst(LList* list);

/**
 * @brief Finds first element for which Comp returns true
 * @param list List
 * @param con Data to compare elements to
 * @return Pointer to first matched data
 */
void *List_GetData(LList *list, const void* con);

/**
 * @param list List
 * @return True if list is empty, otherwise false
 */
bool List_IsEmpty(LList *list);

/**
 * @brief Returns the first element of the list
 * @param list List
 * @return Pointer to this element, if empty, then NULL
 */
void *List_GetFirst(LList *list);

/**
 * @brief Sets first element as active
 * @param list List
 */
void List_SetFirstActive(LList* list);

/**
 * @brief Sets next element as active
 * @param list List
 */
void List_SetNextActive(LList* list);

/**
 * @param list List
 * @return Pointer to the active element's data
 */
void* List_GetActive(LList* list);

#endif // !__LIST__
