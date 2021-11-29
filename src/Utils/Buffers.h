/**
 * @file Buffers.h
 * @author Rastislav Budinsky
 * @brief This file contains struct needed for semantic actions and code generation, interface missing currently
 */
#ifndef __BUFFERS_H__
#define __BUFFERS_H__

#include "vector.h"
#include "symbolelement.h"

typedef struct Buffers_t
{
    Vector *variables;
    Vector *expressions;
    Vector *only_declared;
    int position;
    bool declared;
    Element *current_function;
}Buffers;

/**
 * @brief Constructor for Buffers
 * @return Valid pointer upon success otherwise NULL
 */
Buffers *Buffers_Init();

/**
 * @brief Destructor for Buffers
 * @param buffer Buffers
 */
void Buffers_Destroy(Buffers *buffer);

#endif // !__BUFFERS_H__