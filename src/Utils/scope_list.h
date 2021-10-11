/**
 * @file scope_list.h
 * @brief This file contains interface and struct scope_list, implemented as LIFO
 * @author Rastislav Budinsky
 */
#ifndef __SCOPE_LIST_H__
#define __SCOPE_LIST_H__

#include "symbolelement.h"

struct SListElement;

typedef struct 
{
    struct SListElement *first;
} ScopeList;

/**
 * @brief creates and initializes struct ScopeList
 * @return pointer to the newly created ScopeList
 */
ScopeList *ScopeList_Init();

/**
 * @brief destroys struct ScopeList, should not be used afterwards. NULL pointer has no affect
 * @param list ScopeList to be destroyed
 */
void ScopeList_Destroy(ScopeList *list);

/**
 * @brief Adds new element to the ScopeList
 * @param list ScopeList to which is element added
 * @param element pointer to the element to be added
 */
void ScopeList_AddElement(ScopeList *list, Element *element);

/**
 * @brief Checks if ScopeList is empty
 * @param list ScopeList to be checked
 * @return 1 if empty, otherwise 0
 */
char ScopeList_IsEmpty(ScopeList *list);

/**
 * @brief finds first Element and pops it. Returns this first element
 * @param list ScopeList to get&pop first element from
 * @return pointer to the element if exists, otherwise NULL
 */
Element *ScopeList_PopFirst(ScopeList *list);


#endif // !__SCOPE_LIST_H__