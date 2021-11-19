/**
 * @file data.c
 * @author Rastislav Budinsky
 * @brief This file implements interface for working with data pointer
 */
#include "data.h"
#include "logger.h"

#include <stdlib.h>

typedef struct
{
    SemanticType type;
    int ID;
}Variable;

typedef struct
{
    Vector *params;
    Vector *return_vals;
    int ID;
}Function;

SemanticType *Data_CreateSemantic(SemanticType type)
{
    SemanticType *new_type = (SemanticType*) malloc(sizeof(SemanticType));
    if (new_type == NULL)
        ERROR("Allocation failed");

    *new_type = type;

    return new_type;
}

void Data_FreeSemantic(void * type)
{
    free((SemanticType*)type);
}

void *Data_Init(SymbolType dataType)
{
    void *data = malloc(dataType == VARIABLE ? sizeof(Variable) : sizeof(Function));
    if (data == NULL)
        ERROR("Allocation failed");

    if (dataType == FUNCTION)
    {
        Function *function = (Function*)data;

        function->params = Vector_Init(Data_FreeSemantic);
        if (function->params == NULL)
        {
            free(data);
            return NULL;
        }
        function->return_vals = Vector_Init(Data_FreeSemantic);
        if (function->return_vals == NULL)
        {
            Vector_Destroy(function->params);
            free(data);
            return NULL;
        }

        function->ID = -1;
    }
    else
    {
        ((Variable*)data)->ID = -1;
        ((Variable*)data)->type = SEMANTIC_VOID;
    }

    return data;
}

void Data_Variable_SetSemantic(void *data, SemanticType semanticType)
{
    ((Variable*)data)->type = semanticType;
}

void Data_AddSemanticReturn(void *data, SemanticType semanticType)
{
    SemanticType *new_type = Data_CreateSemantic(semanticType);
    if (new_type == NULL)
        return;
    Vector_PushBack(((Function*)data)->return_vals, new_type);
}

void Data_AddSemanticParam(void *data, SemanticType semanticType)
{
    SemanticType *new_type = Data_CreateSemantic(semanticType);
    if (new_type == NULL)
        return;
    Vector_PushBack(((Function*)data)->params, new_type);
}

SemanticType Data_GetSemanticType(void *data, SymbolType dataType)
{
    if (dataType == VARIABLE)
        return ((Variable*)data)->type;
    else
        return Vector_IsEmpty(((Function*)data)->return_vals) ? SEMANTIC_VOID : *( (SemanticType*) Vector_GetElement(((Function*)data)->return_vals, 0) );
}

Vector *Data_Function_Params(void *data)
{
    return ((Function*)data)->params;
}

Vector *Data_Function_ReturnVals(void *data)
{
    return ((Function*)data)->return_vals;
}

int Data_GetID(void *data, SymbolType dataType)
{
    return dataType == VARIABLE ? ((Variable*)data)->ID : ((Function*)data)->ID;
}

void Data_SetID(void *data, SymbolType dataType, int new_ID)
{
    if (dataType == VARIABLE)
        ((Variable*)data)->ID = new_ID;
    else
        ((Function*)data)->ID = new_ID;
}

void Data_Function_Destroy(void *data)
{
    Function *function = (Function*)data;

    Vector_Destroy(function->params);
    Vector_Destroy(function->return_vals);
    free(function);
}

void Data_ID_Destroy(void *data)
{
    free((Variable*)data);
}