/**
 * @file data.c
 * @author Rastislav Budinsky
 * @brief This file implements interface for working with data pointer
 */
#include "functiondata.h"
#include "logger.h"

#include <stdlib.h>

typedef struct
{
    Vector *params;
    Vector *return_vals;
}Function;

SemanticType *FunctionData_CreateSemantic(SemanticType type)
{
    SemanticType *new_type = (SemanticType*) malloc(sizeof(SemanticType));
    if (new_type == NULL)
        ERROR("Allocation failed");

    *new_type = type;

    return new_type;
}

void FunctionData_FreeSemantic(void * type)
{
    free((SemanticType*)type);
}

void *FunctionData_Init()
{
    void *data = malloc(sizeof(Function));
    if (data == NULL)
        ERROR("Allocation failed");

    Function *function = (Function*)data;
    
    function->params = Vector_Init(FunctionData_FreeSemantic);
    if (function->params == NULL)
    {
        free(data);
        return NULL;
    }
    function->return_vals = Vector_Init(FunctionData_FreeSemantic);
    if (function->return_vals == NULL)
    {
        Vector_Destroy(function->params);
        free(data);
        return NULL;
    }

    return data;
}

void FunctionData_AddSemanticReturn(void *data, SemanticType semanticType)
{
    SemanticType *new_type = FunctionData_CreateSemantic(semanticType);
    if (new_type == NULL)
        return;
    Vector_PushBack(((Function*)data)->return_vals, new_type);
}

void FunctionData_AddSemanticParam(void *data, SemanticType semanticType)
{
    SemanticType *new_type = FunctionData_CreateSemantic(semanticType);
    if (new_type == NULL)
        return;
    Vector_PushBack(((Function*)data)->params, new_type);
}

Vector *FunctionData_Params(void *data)
{
    return ((Function*)data)->params;
}

Vector *FunctionData_ReturnVals(void *data)
{
    return ((Function*)data)->return_vals;
}

void FunctionData_Destroy(void *data)
{
    Function *function = (Function*)data;

    Vector_Destroy(function->params);
    Vector_Destroy(function->return_vals);
    free(function);
}