/**
 * @file jesus.h
 * @brief This file defines interface of struct for holding char* and data
 * @author Rastislav Budinsky
 */
// If jesus doesn't save this shit, then noone can
#ifndef __JESUS_H__
#define __JESUS_H__
#include "stack.h"

typedef struct Christ Jesus;

/**
 * @brief creates and inits jesus struct, allocs new space for id
 * @param id char to be copied
 * @param stack pointer to the initialized stack
 * @return pointer to Jesus
 */
Jesus *Jesus_Init(const char* id, Stack* stack);

/**
 * @param jesus pointer to the struct
 * @return pointer to the saved char*
 */
char *Jesus_GetKey(Jesus *jesus);

/**
 * @param jesus pointer to the struct
 * @return pointer to the stack
 */
Stack *Jesus_GetStack(Jesus *jesus);

/**
 * @param jesus pointer to the struct
 */
void Jesus_Destroy(Jesus *jesus);

#endif // !__JESUS_H__