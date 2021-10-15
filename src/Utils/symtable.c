#include <stdlib.h>
#include <string.h>
#include "symtable.h"
#include "hashtable.h"
#include "jesus.h"
#include "list.h"
#include "stack.h"
#include "logger.h"

// for testing purposes, definitions at the end of file
#include "symtable_testonly.h"

struct SymbolTable
{
    HTable *table;
    Stack *scopes;
    LList *buffer;
    int32_t table_size; // size is already stored in HTable
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

    symtable->scopes = Stack_Init((void ( * ) (void*)) NULL);
    if (symtable->scopes == NULL)
    {
        WARNING("Allocation failed! - Symbol Table");
        goto hashtable;
    }

    symtable->buffer = List_Init((void ( * ) (void*)) List_Destroy, (const bool ( * ) (void*, const void*)) NULL);
    if (symtable->buffer == NULL)
    {
        WARNING("Allocation failed! - Symbol Table");
        goto scopes;
    }

    // create global scope
    Symtable_AddScope(symtable);

    // creating global scope failed
    if (Stack_Top(symtable->scopes) == NULL)
        goto scopes;

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

    Jesus* jesus = (Jesus*) HashTable_Find(symtable->table, id);
    if (jesus == NULL)
    {
        INFO("Identifier not yet in the Symbol Table - list");
        return NULL;
    }

    Stack* stack = (Stack*) Jesus_GetStack(jesus);
    if (stack == NULL)
    {
        INFO("Identifier not yet in the Symbol Table - stack");
        return NULL;
    }

    if (Stack_IsEmpty(stack))
        return NULL;

    return (Element*) Stack_Top(stack);    
}

Element *Symtable_CreateElement(Symtable *symtable, const char *id, int flags)
{
    if (id == NULL)
        ERROR("Invalid parameter!");

    Jesus* jesus = (Jesus*) HashTable_Find(symtable->table, id);
    // not yet in the hash table
    if (jesus == NULL)
    {
        jesus = HashTable_Insert(symtable->table, id);
        if (jesus == NULL)
            return NULL;
    }

    Stack* stack = Jesus_GetStack(jesus);

    if (stack == NULL)
        return NULL;

    Element *element = Element_Init(Jesus_GetKey(jesus), FUNCTION, false, NULL);
    if (element == NULL)
        return NULL;

    Stack_Push(stack, (void*) element);

    // Element was succesfully added to the Hashtable
    // Now we need to add it to the Scope Stack

    // there should be always one scope
    if (Stack_IsEmpty(symtable->scopes))
        ERROR("No scopes are defined!");

    LList *scope_list = (LList*) Stack_Top(symtable->scopes);

    if (List_AddFirst(scope_list, (void*) element) == NULL)
        return NULL;
        
    return element;
}

Element *Symtable_GetElementFromBuffer(Symtable *symtable, const char *id)
{
    // need to iterate through the list and find the right Element
    List_SetFirstActive(symtable->buffer);
    LList* list = (LList*) List_GetActive(symtable->buffer);
    while (list != NULL)
    {
        if ((void*) List_GetData(list, id) != NULL)
            return (Element*) List_GetData(list, id);

        List_SetNextActive(symtable->buffer);
        list = (LList*) List_GetActive(symtable->buffer);
    }      
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
    Jesus *jesus = (Jesus*) HashTable_Find(symtable->table, Element_GetKey(element));
    if (jesus == NULL)
    {
        INFO("Removing element from Hashtable, jesus not found");
        return;
    }

    // finds stack in which our Element is
    Stack *stack = Jesus_GetStack(jesus);
    if (stack == NULL)
    {
        INFO("Removing element from Hashtable, stack not found");
        return;
    }

    Element *top = (Element*) Stack_Top(stack);

    // should not remove element, that does not match
    if (!Element_IsEqual(top, element))
    {
        WARNING("Removing element from Hashtable, top element in stack does not match found element");
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
    LList *list = List_Init((void ( * ) (void*)) NULL, (const bool ( * ) (void*, const void*)) Element_CompareWithString);  
    if (list == NULL)
        ERROR_VOID("Allocation failed");
    
    Stack_Push(symtable->scopes, (void*) list);
}

/**
 * @brief Removes all elements from hash table and pops scopes stack
 * @param symtable Pointer to symtable
 * @param destroy If set to true, frees element and list memory
*/
void Symtable_RemoveScope(Symtable *symtable, bool destroy)
{
    LList* list = (LList*) Stack_Pop(symtable->scopes);
    if (list == NULL)
        ERROR_VOID("Scope stack is empty!");

    if (List_IsEmpty(list))
    {
        WARNING("Clearing empty scope list!");
        return;
    }

    List_SetFirstActive(list);
    Element* data = (Element*)List_GetActive(list);
    while (data != NULL)
    {
        Element_RemoveFromHashTable(symtable, data);
        //destroy the element itself
        if (destroy)
            Element_Destroy(data);

        List_SetNextActive(list);
        data = (Element*)List_GetActive(list);
    }

    if (destroy)
        List_Destroy(list);
    return;
}

void Symtable_PopScope(Symtable *symtable)
{
    Symtable_RemoveScope(symtable, true);
}

bool Symtable_PopScopeToBuffer(Symtable *symtable)
{
    if (Stack_IsEmpty(symtable->scopes))
        ERROR("Working with empty scope stack!");

    LList *list = (LList*) Stack_Top(symtable->scopes);
    
    if (list == NULL)
        ERROR("Received empty list?!");

    Symtable_RemoveScope(symtable, false);

    return List_AddFirst(symtable->buffer, list) == NULL ? false : true;
}

void Symtable_ClearBuffer(Symtable *symtable)
{
    while (!List_IsEmpty(symtable->buffer))
    {
        LList *list = (LList*) List_GetFirst(symtable->buffer);
        List_RemoveFirst(symtable->buffer);

        while (!List_IsEmpty(list))
        {
            Element_Destroy((Element*) List_GetFirst(list));
            List_RemoveFirst(list);
        }
    }
}


HTable *Symtable_GetHashtable(Symtable *symtable)
{
    return symtable->table;
}

Stack *Symtable_GetScopeStack(Symtable *symtable)
{
    return symtable->scopes;
}

LList *Symtable_GetBufferList(Symtable *symtable)
{
    return symtable->buffer;
}