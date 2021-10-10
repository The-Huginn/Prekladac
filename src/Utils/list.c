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
    char *key;
    Stack *data;
    struct ListElement *next;
} LElement;

/**
 * @brief creates and initializes list element and copies string value
 * @param key pointer, where the original string is
 * @param data pointer, where the stack is
 * @return newly created list element
 */
LElement *LElement_Init(const char *key, Stack *data)
{
    LElement *element = (LElement*) malloc(sizeof(LElement));
    if (element == NULL)
        ERROR("Allocation failed!");

    char* new_key = strdup(key);
    if (new_key == NULL)
        ERROR("Allocation failed!");
    
    element->key = new_key;
    element->data = data;
    element->next = NULL;

    return element;
}

/**
 * @brief destructor for list element, it's string key should not be accessed from another struct
 * @param element the list element to be destroyed
 */
void LElement_Destroy(LElement *element)
{
    free(element->key);
    Stack_Destroy(element->data);
    free(element);
}

LList *List_Init()
{
    LList *list = (LList*) malloc(sizeof(LList));

    if (list == NULL)
        ERROR("Allocation failed!");

	list->begin = NULL;

    return list;
}

void List_Destroy(LList *list)
{
    if (list == NULL)
        ERROR_VOID("Destroying uninitialized list!");

    while(list->begin != NULL)
    {
        LElement *tmp = list->begin->next;
        LElement_Destroy(list->begin);
        list->begin = tmp;
    }
}

Stack *List_GetStack(LList *list, const char *key)
{
    if (list == NULL || key == NULL)
        ERROR("Invalid argument!");

    LElement *tmp = list->begin;

    while(tmp != NULL)
    {
        // indentical
        if (strcmp(key, tmp->key) == 0)
            return tmp->data;

        tmp = tmp->next;
    }

    // we didnt find valid element
    Stack *stack = Stack_Init();
    if (stack == NULL)
        ERROR("Allocation failed!");

    LElement *element = LElement_Init(key, stack);
    if (element == NULL)
        ERROR("Allocation failed!");
    
    element->next = list->begin;
    list->begin = element;
    return element->data;
}