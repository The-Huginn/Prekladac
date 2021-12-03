// IFJ Project 2021
/**
 * @file vector.c
 * @author Rastislav Budinsky (xbudin05)
 * @brief This file implements the vector class and all it's interface functions
 */
#include "vector.h"
#include "logger.h"

// for testing purposes only
#include "vector_testonly.h"

#include <stdlib.h>

#define VECTOR_INIT_SIZE 10

struct Vector_t
{
    void** array;
    void (*VectorElementDtor)(void*);
    int index;
    int maxSize;
};

Vector *Vector_Init(void (*VectorElementDtor)(void*))
{
    Vector *vector = (Vector*) malloc(sizeof(struct Vector_t));
    if (vector == NULL)
        ERROR("Allocation failed");

    vector->array = (void**) malloc(sizeof(void*) * VECTOR_INIT_SIZE);
    if (vector->array == NULL)
    {
        free(vector);
        ERROR("Allocation failed");
    }

    vector->maxSize = VECTOR_INIT_SIZE;
    vector->index = -1;
    vector->VectorElementDtor = VectorElementDtor;

    return vector;
}

void Vector_Destroy(Vector *vector)
{
    Vector_Clear(vector);

    if (vector->array != NULL)
        free(vector->array);

    free(vector);
}

bool Vector_Clear(Vector *vector)
{
    if (vector->VectorElementDtor == NULL)
        WARNING("Missing Dtor");
    else
        for (int i = 0; i < Vector_Size(vector); i++)
            vector->VectorElementDtor(vector->array[i]);

    vector->index = -1;
    vector->maxSize = VECTOR_INIT_SIZE;
    vector->array = (void**) realloc(vector->array, sizeof(void*) * VECTOR_INIT_SIZE);

    if (vector->array == NULL)
        ERROR("Reallocation failed");

    return true;
}

bool Vector_PushBack(Vector *vector, void* data)
{
    if (Vector_Size(vector) == vector->maxSize)
    {
        vector->maxSize *= 2;
        vector->array = (void**) realloc(vector->array, sizeof(void*) * vector->maxSize);
        if (vector->array == NULL)
            ERROR("Reallocation failed");
    }

    vector->array[++(vector->index)] = data;

    return true;
}

bool Vector_PopBack(Vector *vector)
{
    if (vector->VectorElementDtor != NULL)
        vector->VectorElementDtor(vector->array[vector->index]);

    vector->index--;

    if (Vector_Size(vector) < vector->maxSize / 4 && vector->maxSize > 2 * VECTOR_INIT_SIZE) // to not reallocate with small vector
    {
        vector->maxSize /= 2;
        vector->array = (void**)realloc(vector->array, sizeof(void*) * vector->maxSize);
        if (vector->array == NULL)
            ERROR("Reallocation failed");
    }

    return true;
}

void *Vector_Back(Vector *vector)
{
    return vector->index == -1 ? NULL : vector->array[vector->index];
}

bool Vector_IsEmpty(Vector *vector)
{
    return vector->index == -1;
}

int Vector_Size(Vector *vector)
{
    return vector->index + 1;
}

void* Vector_GetElement(Vector *vector, int index)
{
    if (index < 0 || index > vector->index)
        return NULL;

    return vector->array[index];
}

bool Vector_InsertElement(Vector *vector, int index, void* data)
{
    if (index < 0 || index > vector->index + 1)
        return false;

    if (Vector_Size(vector) == vector->maxSize)
    {
        vector->maxSize *= 2;
        vector->array = (void**) realloc(vector->array, sizeof(void*) * vector->maxSize);
        if (vector->array == NULL)
            ERROR("Reallocation failed");
    }

    vector->index++;

    for (int i = Vector_Size(vector) - 1; i > index; i--)
        vector->array[i] = vector->array[i - 1];

    vector->array[index] = data;

    return true;
}

bool Vector_RemoveElement(Vector *vector, int index)
{
    if (index < 0 || index > vector->index)
        return true;

    if (vector->VectorElementDtor != NULL)
        vector->VectorElementDtor(vector->array[index]);

    vector->index--;

    for (int i = index; i < Vector_Size(vector); i++)
        vector->array[i] = vector->array[i + 1];

    if (Vector_Size(vector) < vector->maxSize / 4 && vector->maxSize > 2 * VECTOR_INIT_SIZE) // to not reallocate with small vector
    {
        vector->maxSize /= 2;
        vector->array = (void**)realloc(vector->array, sizeof(void*) * vector->maxSize);
        if (vector->array == NULL)
            ERROR("Reallocation failed");
    }

    return true;    
}

// for testing purposes only
void **Vector_GetArray(Vector* vector)
{
    return vector->array;
}

