/**
 * @file scope_list.c
 * @brief This file implements interface of scope_list.h
 * @author Rastislav Budinsky
 */
#include <stdlib.h>
#include "scope_list.h"
#include "logger.h"

typedef struct SListElement
{
    Element *data;
    struct SListElement *next;
} SLElement;

SLElement *SLElement_Init(Element *element, SLElement *next)
{
    SLElement *new_element = (SLElement*) malloc(sizeof(SLElement));
    if (new_element == NULL)
        ERROR("Allocation failed!");

    new_element->data = element;
    new_element->next = next;
}

void SLElement_Destroy(SLElement *element)
{
    if (element != NULL)
        free(element);
}

ScopeList *ScopeList_Init()
{
    ScopeList *list = (ScopeList*) malloc(sizeof(ScopeList));
    if (list == NULL)
        ERROR("Allocation failed!");

    list->first = NULL;

    return list;
}

void ScopeList_Destroy(ScopeList *list)
{
    if (list != NULL)
        free(list);
}

void ScopeList_AddElement(ScopeList *list, Element *element)
{
    if (list == NULL || element == NULL)
        ERROR_VOID("Invalid parameter!");

    SLElement *new_element = SLElement_Init(element, list->first);
    
    if (new_element != NULL)
        list->first = new_element;
}

char ScopeList_IsEmpty(ScopeList *list)
{
    if (list == NULL)
        ERROR("Invalid parameter!");

    return list->first == NULL ? 1 : 0;
}

Element *ScopeList_PopFirst(ScopeList *list)
{
    if (list == NULL)
        ERROR("Invalid parameter!");

    if (list->first == NULL)
    {
        WARNING("No more elements in ScopeList!");
        return NULL;
    }

    Element *to_return = list->first->data;
    SLElement *to_delete = list->first;
    list->first = to_delete->next;

    SLElement_Destroy(to_delete);

    return to_return;
}