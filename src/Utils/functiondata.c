/**
 * @file data.c
 * @author Rastislav Budinsky
 * @brief This file implements interface for working with data pointer
 */
#include "functiondata.h"
#include "logger.h"

// for MAX_LEXEME_LEN macro only
#include "../LexicalAnalyzer/LexicalFSM.h"

#include <stdlib.h>

typedef struct
{
    Vector *params;
    Vector *return_vals;
}Function;

typedef struct
{
    SemanticType semanticType;
    char id[MAX_LEXEME_LEN + 1];
}FunctionParam;

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

FunctionParam *FunctionData_CreateParam(char *id, SemanticType type)
{
    FunctionParam *param = (FunctionParam*) malloc(sizeof(FunctionParam));
    if (param == NULL)
        ERROR("Allocation failed");

    param->semanticType = type;

    // we dont know the name if only declared
    if (id == NULL)
    {
        param->id[0] = '\0';
        return param;
    }

    for (int i = 0; i < MAX_LEXEME_LEN; i++)
    {
        param->id[i] = id[i];

        // \0 should be copied as well
        if (id[i] == '\0')
            break;
    }

    return param;
}

void FunctionData_FreeParam(void *param)
{
    free(param);
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
    function->return_vals = Vector_Init(FunctionData_FreeParam);
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

void FunctionData_AddSemanticParam(void *data, SemanticType semanticType, char* id)
{
    FunctionParam *new_type = FunctionData_CreateParam(id, semanticType);
    if (new_type == NULL)
        return;
    Vector_PushBack(((Function*)data)->params, new_type);
}

int FunctionData_Params_Size(void *data)
{
    return Vector_Size(((Function*)data)->params);
}

int FunctionData_Returns_Size(void *data)
{
    return Vector_Size(((Function*)data)->return_vals);
}

char *FunctionData_Params_GetName(void *data, int index)
{
    if (Vector_Size(((Function*)data)->params) <= index)
        return NULL;

    return ((FunctionParam*)Vector_GetElement(((Function*)data)->params, index))->id;
}

void FunctionData_Params_SetName(void *data, int index, const char *id)
{
    if (Vector_Size(((Function*)data)->params) <= index)
        ERROR_VOID("Invalid index");

    if (id == NULL)
        ERROR_VOID("Changin name to NULL");

    char *ptr = ((FunctionParam*)Vector_GetElement(((Function*)data)->params, index))->id;
    for(int i = 0; i < MAX_LEXEME_LEN; i++)
    {
        ptr[i] = id[i];

        if (id[i] == '\0')
            break;
    }
}

void FunctionData_Params_SetSemantic(void *data, int index, SemanticType semanticType)
{
    if (Vector_Size(((Function*)data)->params) <= index)
        ERROR_VOID("Invalid index");

    ((FunctionParam*)Vector_GetElement(((Function*)data)->params, index))->semanticType = semanticType;
}

SemanticType FunctionData_Params_GetSemantic(void *data, int index)
{
    if (Vector_Size(((Function*)data)->params) <= index)
        return SEMANTIC_VOID;

    return ((FunctionParam*)Vector_GetElement(((Function*)data)->params, index))->semanticType;
}

SemanticType FunctionData_Returns_GetSemantic(void *data, int index)
{
    return *((SemanticType*) Vector_GetElement(((Function*)data)->return_vals, index));
}

void FunctionData_Destroy(void *data)
{
    Function *function = (Function*)data;

    Vector_Destroy(function->params);
    Vector_Destroy(function->return_vals);
    free(function);
}