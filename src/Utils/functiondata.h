/**
 * @file functiondata.h
 * @author Rastislav Budinsky
 * @brief This file contains interface for working with data of ID / FUNCTION
 */
#ifndef __FUNCTION_DATA_H__
#define __FUNCTION_DATA_H__

#include "semantictype.h"
#include "symbolelement.h"
#include "vector.h"

/**
 * @return void* 
 */
void *FunctionData_Init();

/**
 * @param data Data
 * @param semanticType SemanticType
 */
void FunctionData_AddSemanticReturn(void *data, SemanticType semanticType);

/**
 * @param data Data
 * @param semanticType SemanticType
 */
void FunctionData_AddSemanticParam(void *data, SemanticType semanticType);

/**
 * @param data Data
 * @return pointer to the Vector of SemanticType* of parameters. @note Do not change content
 */
Vector *FunctionData_Params(void *data);

/**
 * @param data Data
 * @return pointer to the Vector of SemanticType* of return values. @note Do not change content
 */
Vector *FunctionData_ReturnVals(void *data);

/**
 * @brief Destroy function for Function data
 * @param data Data
 */
void FunctionData_Destroy(void *data);

#endif //!__FUNCTION_DATA_H__