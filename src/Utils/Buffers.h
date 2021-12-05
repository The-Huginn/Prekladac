// IFJ Project 2021
/**
 * @file Buffers.h
 * @author Rastislav Budinsky (xbudin05)
 * @brief This file contains struct needed for semantic actions and code generation, interface missing currently
 */
#ifndef __BUFFERS_H__
#define __BUFFERS_H__

#include "vector.h"
#include "stack.h"
#include "symbolelement.h"

#include <stdio.h>

typedef struct Buffers_t
{
    Vector *variables;  //! used for parsing and saving recent variables from statement
    Vector *expressions;    //! used for parsing and saving recent expressions from statement
    Vector *only_declared;  //! for only declared function, we shoul check if they have been declared
    int position;   //! auxiliary variable for parsing
    bool declared;  //! auxilary variable for function definition to decide, whether it already has signature
    Element *current_function;  //! currently parsed function definition, for semantic actions upon return
    
    Stack *scopes;     //! to remember currently code-branches @note used as stack
    int top_id;     //! id to be assigned to the new code-branch, might be for each code-branch type different
    int tmp_offset; //! auxilary variable for generating unique variable names for expression generations
    FILE *output;   //! output where code is generated to

    Vector *function_calls; //! for all function calls in global scope
}Buffers;

/**
 * @brief Constructor for Buffers
 * @param output Output file where code is generated
 * @return Valid pointer upon success otherwise NULL
 */
Buffers *Buffers_Init(FILE *output);

/**
 * @brief Destructor for Buffers
 * @param buffer Buffers
 */
void Buffers_Destroy(Buffers *buffer);

/**
 * @brief Clears buffer after statement
 * @param buffer Buffers
 */
void Buffers_Clear(Buffers *buffer);

#endif // !__BUFFERS_H__