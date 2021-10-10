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
        ERROR("Allocation failed!\n");

    char* new_key = (char*) malloc(strlen(key));
    if (new_key == NULL)
        ERROR("Allocation failed!\n");

    strcpy(new_key, key);
    
    element->key = new_key;
    element->data = data;
    element->next = NULL;
}

/**
 * @brief destructor for list element, it's string key should not be accessed from another struct
 * @param element the list element to be destroyed
 */
void LElement_Free(LElement *element)
{
    free(element->key);
    free(element);
}

LList *List_Init()
{
    LList *list = (LList*) malloc(sizeof(LList));

    if (list == NULL)
        ERROR("Allocation failed!\n");

	list->begin = NULL;

    return list;
}

void List_Destroy(LList *list)
{
    if (list == NULL)
        ERROR("Destroying uninitialized list!\n");

    while(list->begin != NULL)
    {
        LElement *tmp = list->begin->next;
        Stack_Destroy(list->begin->data);
        LElement_Free(list->begin);
        list->begin = tmp;
    }
}

Stack *List_GetStack(LList *list, const char *key)
{
    if (list == NULL || key == NULL)
        ERROR("Invalid argument!\n");

    LElement *tmp = list->begin;

    while(tmp != NULL)
    {
        // indentical
        if (strcmp(key, tmp->key) == 0)
            return tmp->data;

        // break at last valid element
        if (tmp->next == NULL)
            break;
    }

    // we didnt find valid element
    Stack *stack = Stack_Init();
    if (stack == NULL)
        ERROR("Allocation failed!\n");

    LElement *element = LElement_Init(key, stack);
    if (element == NULL)
        ERROR("Allocation failed!\n");
    
    if (tmp == NULL)
        list->begin = element;
    
    tmp->next = element;
}