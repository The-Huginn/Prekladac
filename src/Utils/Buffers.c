/**
 * @file Buffers.c
 * @author
 * @brief 
 */
#include "Buffers.h"

#include <stdlib.h>
#include <stdbool.h>

void ScopeDtor(void *data)
{
    free(data);
}

Buffers *Buffers_Init(FILE *output)
{
    Buffers *buffer = (Buffers*)malloc(sizeof(Buffers));
    if (buffer == NULL)
        return NULL;

    buffer->variables = Vector_Init((void (*)(void*)) NULL);
    if (buffer->variables == NULL)
    {
        free(buffer);
        return NULL;
    }

    buffer->expressions = Vector_Init((void (*)(void*)) NULL);
    if (buffer->expressions == NULL)
    {
        Vector_Destroy(buffer->variables);
        free(buffer);
        return NULL;
    }

    buffer->only_declared = Vector_Init((void (*)(void *))NULL);
    if (buffer->only_declared == NULL)
    {
        Vector_Destroy(buffer->variables);
        Vector_Destroy(buffer->expressions);
        free(buffer);
        return NULL;
    }

    // See ../SyntaxAnalyzer/CodeGeneration needs free-ing function
    buffer->scopes = Stack_Init(ScopeDtor);
    if (buffer->scopes == NULL)
    {
        Vector_Destroy(buffer->variables);
        Vector_Destroy(buffer->expressions);
        Vector_Destroy(buffer->only_declared);
        free(buffer);
        return NULL;
    }

    buffer->function_calls = Vector_Init((void (*)(void *))NULL);
    if (buffer->function_calls == NULL)
    {
        Vector_Destroy(buffer->variables);
        Vector_Destroy(buffer->expressions);
        Vector_Destroy(buffer->only_declared);
        Stack_Destroy(buffer->scopes);
        free(buffer);
        return NULL;
    }

    buffer->position = 0;
    buffer->declared = false;
    buffer->current_function = NULL;
    buffer->top_id = 0;
    buffer->tmp_offset = 0;
    buffer->output = output;

    return buffer;
}

void Buffers_Destroy(Buffers *buffer)
{
    Vector_Destroy(buffer->variables);
    Vector_Destroy(buffer->expressions);
    Vector_Destroy(buffer->only_declared);
    Stack_Destroy(buffer->scopes);
    Vector_Destroy(buffer->function_calls);
    free(buffer);
}