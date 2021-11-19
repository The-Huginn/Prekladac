/**
 * @file symtable.h
 * @brief This file contains interface for working with the Symbol Table
 * @author Rastislav Budinsky
 */
#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__

#include <stdbool.h>
#include "symbolelement.h"

typedef struct SymbolTable Symtable;

/**
 * @brief Creates and initializes Symbol table, creates global scope
 * @return pointer to the Symbol table
 */
Symtable *Symtable_Init();

/**
 * @brief Destroys the Symbol table, no Elements should be accessed afterwards
 * @param symtable Symbol table
 */
void Symtable_Destroy(Symtable *symtable);

/**
 * @brief Finds the deepest declared identifier
 * @param symtable Symbol table
 * @param id the identifier
 * @return pointer to the deepest declared id
 */
Element *Symtable_GetElement(Symtable *symtable, const char* id);

/**
 * @brief Creates an Element, saves it into the Symbol table and returns pointer to Element
 *      For further definition of Element symbolelement.h interface should be used
 * @param symtable Symbol table
 * @param id the identifier
 * @param type The type of variable
 * @return Pointer to the newly created Element
 */
Element *Symtable_CreateElement(Symtable *symtable, const char* id, SymbolType type);

/**
 * @brief Finds the Element in the buffer i.e. Element that is not visible from this scope but was in scope before
 * @param symtable Symbol table
 * @param id the identifier
 * @return Pointer to the first found Element, if nothing found then NULL
 */
Element *Symtable_GetElementFromBuffer(Symtable *symtable, const char* id);

/**
 * @brief Creates new item on top of stack for Scopes, i.e. new comming Elements will be written to deeper scope
 * @param symtable Symbol table
 */
void Symtable_AddScope(Symtable *symtable);

/**
 * @brief Removes top scope from stack and destroys all of this scope Elements
 * @param symtable Symbol table
 */
void Symtable_PopScope(Symtable *symtable);

/**
 * @brief Removes top scope from stack to buffer, Elements not destroyed
 * @param symtable Symbol table
 * @return True upon success, otherwise false
 */
bool Symtable_PopScopeToBuffer(Symtable *symtable);

/**
 * @brief Clears all elements from the buffer, calling upon empty buffer prints warning
 * @param symtable Symbol table
 */
void Symtable_ClearBuffer(Symtable *symtable);

#endif // !__SYMTABLE_H__