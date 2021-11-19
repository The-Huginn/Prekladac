/**
 * @file vector_testonly.h
 * @author Rastislav Budinsky
 * @brief This file should be used only upon testing
 */
#include "vector.h"

/**
 * @param vector Vector
 * @return Array of elements
 */
void** Vector_GetArray(Vector* vector);