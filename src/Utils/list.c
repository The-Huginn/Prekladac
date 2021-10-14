/**
 * @file list.c
 * @brief This file contains implementation of internal structure of elements
 * and interface of it's header file
 * @author Rastislav Budinsky
 */
#include <string.h>

#include "list.h"
#include "logger.h"

typedef struct ListElement
{
    void *data;
    struct ListElement *next;
} LElement;

/**
 * @brief creates and initializes list element and copies string value
 * @param key pointer, where the original string is
 * @param data pointer, where the stack is
 * @return newly created list element
 */
LElement *LElement_Init(void *data)
{
    LElement *element = (LElement*) malloc(sizeof(LElement));
    if (element == NULL)
        ERROR("Allocation failed!");
    
    element->data = data;
    element->next = NULL;

    return element;
}

/**
 * @brief destructor for list element, it's string key should not be accessed from another struct
 * @param element the list element to be destroyed
 */
void LElement_Destroy(LElement *element, void (*DataDtor)(void*))
{
    if (DataDtor == NULL)
        WARNING("Data were not freed!");
    else
        DataDtor(element->data);
    free(element);
}

LList *List_Init(void (*DataDtor)(void*), const bool (*Comp)(void*, const void*))
{
    if (Comp == NULL)
        WARNING("Missing comparator function!");

    LList *list = (LList*) malloc(sizeof(LList));

    if (list == NULL)
        ERROR("Allocation failed!");

    list->DataDtor = DataDtor;
    list->Comp = Comp;
	list->begin = NULL;
    list->active = NULL;

    return list;
}

void List_Destroy(LList *list)
{
    if (list == NULL)
        ERROR_VOID("Destroying uninitialized list!");

    List_Clear(list);
    free(list);
}

void List_Clear(LList* list)
{
    if (list == NULL)
        ERROR_VOID("Invalid argument!");

    while (list->begin != NULL)
    {
        LElement* tmp = list->begin->next;
        LElement_Destroy(list->begin, list->DataDtor);
        list->begin = tmp;
    }
    return;
}

void* List_AddFirst(LList* list, void* data)
{
    if (list == NULL || data == NULL)
        ERROR("Invalid argument!");

    LElement* new_element = LElement_Init(data);

    new_element->next = list->begin;
    list->begin = new_element;

    return new_element;
}

void List_RemoveFirst(LList* list)
{
    if (list == NULL)
        ERROR_VOID("Invalid argument!");

    LElement* temp = list->begin->next;
    LElement_Destroy(list->begin, list->DataDtor);
    list->begin = temp;

    return;
}

void *List_GetData(LList *list, const void* con)
{
    if (list == NULL)
        ERROR("Invalid argument!");
    
    if (list->Comp == NULL)
        ERROR("Missing comparator function!");

    if (con == NULL)
        WARNING("Conparing to NULL!");
        
    LElement *tmp = list->begin;

    while(tmp != NULL)
    {
        if (list->Comp(tmp->data, con))
            return tmp->data;

        tmp = tmp->next;
    }
    return NULL;
}

bool List_IsEmpty(LList *list)
{
    return list->begin == NULL ? true : false;
}

void *List_GetFirst(LList *list)
{
    return list->begin;
}

void List_SetFirstActive(LList* list)
{
    if (list == NULL)
        ERROR_VOID("Invalid argument!");

    list->active = list->begin;
    return;
}

void List_SetNextActive(LList* list)
{
    if (list == NULL)
        ERROR_VOID("Invalid argument!");

    if (list->active == NULL)
        ERROR_VOID("List is not active!");

    list->active = list->active->next;
    return;
}

void* List_GetActive(LList* list)
{
    if (list == NULL)
        ERROR("Invalid argument!");

    if (list->active == NULL)
        ERROR("List is not active!");

    return list->active->data;
}
