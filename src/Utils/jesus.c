// IFJ Project 2021
/**
 * @file jesus.c
 * @brief This file cointains implementation of struct and interface from jesus.h
 * @author Rastislav Budinsky (xbudin05)
 */
#include <stdlib.h>
#include <string.h>
#include "jesus.h"
#include "logger.h"

typedef struct Christ
{
    char* key;
    Stack *stack;
}Jesus;

Jesus *Jesus_Init(const char *id, Stack *stack)
{
    Jesus *jesus = (Jesus*) malloc(sizeof(Jesus));
    if (jesus == NULL)
        ERROR("Allocation failed!");


    jesus->key = strdup(id);
    if (jesus->key == NULL)
    {
        free(jesus);
        ERROR("Allocation failed!");
    }

    jesus->stack = stack;

    return jesus;
}

void Jesus_Destroy(Jesus *jesus)
{
    Stack_Destroy(jesus->stack);
    free(jesus->key);
    free(jesus);
}

char *Jesus_GetKey(Jesus *jesus)
{
    return jesus->key;
}

Stack *Jesus_GetStack(Jesus *jesus)
{
    return jesus->stack;
}