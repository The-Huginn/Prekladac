#include <stdlib.h>
#include <string.h>
#include "symtable.h"
#include "hashtable.h"
#include "list.h"
#include "stack.h"
#include "logger.h"

struct SymbolTable
{
    HTable *table;
    Stack *scopes;
    LList *buffer;
    int32_t table_size;
};

Symtable *Symtable_Init()
{
    Symtable *symtable = (Symtable*) malloc(sizeof(Symtable));
    if (symtable == NULL)
        ERROR("Allocation failed!");
    
    symtable->table_size = 1000000007;

    symtable->table = HashTable_Init(symtable->table_size);
    if (symtable->table == NULL)
    {
        WARNING("Allocation failed! - Symbol Table");
        goto symtable;
    }

    symtable->scopes = Stack_Init((void ( * ) (void*)) List_Destroy);
    if (symtable->scopes == NULL)
    {
        WARNING("Allocation failed! - Symbol Table");
        goto hashtable;
    }

    symtable->buffer = List_Init((void ( * ) (void*)) List_Destroy, (bool ( * ) (void*, void*)) NULL);
    if (symtable->buffer == NULL)
    {
        WARNING("Allocation failed! - Symbol Table");
        goto scopes;
    }

    // here is succesful return
    return symtable;

    scopes:
    free(symtable->scopes);

    hashtable:
    free(symtable->table);

    symtable:
    free(symtable);

    return NULL;
}

void Symtable_Destroy(Symtable *symtable)
{
    // clear all Elements from active stack
    while (!Stack_IsEmpty(symtable->scopes))
        Symtable_PopScope(symtable);

    // clear all Elements from buffer
    Symtable_ClearBuffer(symtable);

    // now table, scopes and buffer should be cleared from Elements
    HashTable_Destroy(symtable->table);
    Stack_Destroy(symtable->scopes);
    List_Destroy(symtable->buffer);

    // finally free struct Symtable
    free(symtable);
}

Element *Symtable_GetElement(Symtable *symtable, const char *id)
{
    if (id == NULL)
        ERROR("Invalid parameter!");

    LList* list = (LList*) HashTable_Find(symtable->table, id);
    if (list == NULL)
    {
        INFO("Identifier not yet in the Symbol Table - list");
        return NULL;
    }

    Stack* stack = (Stack*) List_GetData(list, (void*) id);
    if (stack == NULL)
    {
        INFO("Identifier not yet in the Symbol Table - stack");
        return NULL;
    }

    return (Element*) Stack_Top(stack);    
}

Element *Symtable_CreateElement(Symtable *symtable, const char *id, int flags)
{
    if (id == NULL)
        ERROR("Invalid parameter!");

    LList* list = (LList*) HashTable_Find(symtable->table, id);
    // not yet in the hash table
    if (list == NULL)
    {
        list = HashTable_Insert(symtable->table, id);
        if (list == NULL)
            return NULL;
    }

    Stack* stack = (Stack*) List_GetData(list, (void*) id);

    // not yet initialized, I should initialize it, via the pointer
    if (stack == NULL)
    {
        stack = Stack_Init((void ( * ) (void*)) NULL);  // clearing of stack is done in different functions
        if (stack == NULL)
            return NULL;

        // add the stack to the list
        List_AddFirst(list, (void*)stack);
    }

    Element *element = Element_Init(id, FUNCTION, false, NULL);
    if (element == NULL)
        return NULL;

    Stack_Push(stack, (void*) element);

    // Element was succesfully added to the Hashtable
    // Now we need to add it to the Scope Stack

    // there should be always one scope
    if (Stack_IsEmpty(symtable->scopes))
        ERROR("No scopes are defined!");

    LList *scope_list = (LList*) Stack_Top(stack);

    return (Element*) List_AddFirst(scope_list, (void*) element);
}

Element *Symtable_GetElementFromBuffer(Symtable *symtable, const char *id)
{
    // need to iterate through the list and find the right Element
    // LList *list = List_GetFirst(symtable->buffer);
    // while (list != NULL)
    // {
    //     if ((void*) List_GetData(list, id) != NULL)
    //         return (Element*) List_GetData(list, id);
    // }
    return NULL;
}

/**
 * @brief this function removes the element from the HashTable, meaning it removes it's pointer from the bottom stack
 * @param symtable the Symbol Table
 * @param element the Element to be removed from the stack
 */
void Element_RemoveFromHashTable(Symtable *symtable, Element *element)
{
    // finds list in which our Element is
    LList *list = (LList*) HashTable_Find(symtable->table, (void*) Element_GetKey(element));
    if (list == NULL)
    {
        INFO("Removing element from Hashtable, list not found");
        return;
    }

    // finds stack in which our Element is
    Stack *stack = (Stack*) List_GetData(list, (void*) Element_GetKey(element));
    if (stack == NULL)
    {
        INFO("Removing element from Hashtable, stack not found");
        return;
    }

    Element *top = (Element*) Stack_Top(stack);

    // should not remove element, that does not match
    if (!Element_IsEqual(top, element))
    {
        INFO("Removing element from Hashtable, top element in stack does not match found element");
        return;
    }

    Stack_Pop(stack);
}

bool Element_CompareWithString(Element *element, const char *key)
{
    if (element == NULL)
        return false;

    return strcmp(Element_GetKey(element), key) == 0 ? true : false;
}

void Symtable_AddScope(Symtable *symtable)
{
    // defaultly we want to only remove the element from the Hashtable, not destroy it (for case of buffer)
    // in this list we want to only know, if element was or was not declared in this scope
    LList *list = List_Init((void ( * ) (void*)) Element_RemoveFromHashTable, (bool ( * ) (void*, void*)) Element_CompareWithString);
    if (list == NULL)
        ERROR_VOID("Allocation failed");
    
    Stack_Push(symtable->scopes, (void*) list);
}

bool Symtable_ClearList(LList *list, Symtable *symtable)
{
    if (list == NULL)
        ERROR("Received empty list?!");

    while (!List_IsEmpty(list))
    {
        Element *element = List_GetFirst(list);
        if (element == NULL)
            ERROR("Can not destroy NULL!");

        // remove the element from the hashtable
        List_RemoveFirst(list);

        //destroy the element itself
        Element_Destroy(element);
    }

    // destroy list itself
    List_Destroy(list);
}

void Symtable_PopScope(Symtable *symtable)
{
    LList *list = (LList*) Stack_Pop(symtable->scopes);

    (void) Symtable_ClearList(list, symtable);
}

bool Symtable_PopScopeToBuffer(Symtable *symtable)
{
    if (Stack_IsEmpty(symtable->scopes))
        ERROR("Working with empty scope stack!");

    LList *list = (LList*) Stack_Pop(symtable->scopes);
    
    if (list == NULL)
        ERROR("Received empty list?!");

    return List_AddFirst(symtable->buffer, list) == NULL ? false : true;
}

void Symtable_ClearBuffer(Symtable *symtable)
{
    while (!List_IsEmpty(symtable->buffer))
    {
        LList *list = List_GetFirst(symtable->buffer);
        (void) Symtable_ClearList(list, symtable);
    }
}