/**
 * @file symtable_testonly.h
 * @brief This file expands interface of symtable, this file should be included for test purposes only
 * @author Rastislav Budinsky
 */
#ifndef __SYMTABLE_TESTONLY_H__
#define __SYMTABLE_H__
#include "symtable.h"
#include "list.h"
#include "stack.h"
#include "hashtable.h"

typedef struct SymbolTable Symtable;

/**
 * @brief creates and initializes Symbol table, creates global scope
 * @return pointer to the Symbol table
 */
Symtable *Symtable_Init();

/**
 * @brief Destroys the Symbol table, no Elements should be accessed afterwards
 * @param symtable pointer to the Symbol table
 */
void Symtable_Destroy(Symtable *symtable);

/**
 * @brief finds the deepest definition of identifier
 * @param symtable pointer to the Symbol table
 * @param id the identifier
 * @return pointer to the deepest definition of id
 */
Element *Symtable_GetElement(Symtable *symtable, const char* id);

/**
 * @brief creates the Element, saves it into the Symbol table and returns pointer to Element
 * @param symtable pointer to the Symbol table
 * @param id the identifier
 * @param flags the flags for initial setup
 * @return pointer to the newly created Element
 */
Element *Symtable_CreateElement(Symtable *symtable, const char* id, int flags);

/**
 * @brief finds the Element in the buffer i.e. Element that is not visible from this scope but was above
 * @param symtable pointer to the Symbol table
 * @param id the identifier
 * @return pointer to the first found Element, if nothing found then NULL
 */
Element *Symtable_GetElementFromBuffer(Symtable *symtable, const char* id);

/**
 * @brief creates new item on top of stack for Scopes, i.e. new comming Elements will be written to deeper scope
 * @param symtable pointer to the Symbol table
 */
void Symtable_AddScope(Symtable *symtable);

/**
 * @brief Removes top item stack stack i.e. remove the deepest scope and delete all elements in this scope
 * @param symtable pointer to the Symbol table
 */
void Symtable_PopScope(Symtable *symtable);

/**
 * @brief Removes top item from stack i.e. remove the deepest scope but keep all elements in the buffer, should be cleared to clear elements
 * @param symtable pointer to the Symbol table
 * @return true upon success, otherwise false
 */
bool Symtable_PopScopeToBuffer(Symtable *symtable);

/**
 * @brief Clears all elements from the buffer, calling upon empty buffer prints warning
 * @param symtable pointer to the Symbol table
 */
void Symtable_ClearBuffer(Symtable *symtable);

/**
 * @param symtable the Symbol table
 * @return the pointer to the hashtable inside symtable
 */
HTable *Symtable_GetHashtable(Symtable *symtable);

/**
 * @param symtable the Symbol table
 * @return the pointer to the stack of scopes inside symtable
 */
Stack *Symtable_GetScopeStack(Symtable *symtable);

/**
 * @param symtable the Symbol table
 * @return the pointer to the buffer list inside symtable
 */
LList *Symtable_GetBufferList(Symtable *symtable);




#endif // !__SYMTABLE_H__