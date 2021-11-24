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
 * @param id Pointer to the char* which is then copied, set to NULL if yet unknown
 */
void FunctionData_AddSemanticParam(void *data, SemanticType semanticType, char *id);

/**
 * @param data Data
 * @return Count of parameters
 */
int FunctionData_Params_Size(void *data);

/**
 * @param data Data
 * @return Count of returns
 */
int FunctionData_Returns_Size(void *data);

/**
 * @param data Data
 * @param index Index of wanted parameters name
 * @return pointer to the name of indexed parameter. @note Do not change content
 */
char *FunctionData_Params_GetName(void *data, int index);

/**
 * @brief changes name of identifier on index
 * @param data Data
 * @param index Index of wanted parameter
 * @param id new name
 */
void FunctionData_Params_SetName(void *data, int index, const char *id);

/**
 * @brief Changes Semantic type of identifier on index
 * @param data Data
 * @param index Index of wanted parameter
 * @param semanticType new Semantic type
 */
void FunctionData_Params_SetSemantic(void *data, int index, SemanticType semanticType);

/**
 * @param data Data
 * @param index Index of wanted parameters Semantic type
 * @return Semantic type of indexed parameter @note Do not change content
 */
SemanticType FunctionData_Params_GetSemantic(void *data, int index);

/**
 * @param data Data
 * @param index Index of wanted parameter Semantic type
 * @return Semantic type of indexed parameter @note Do not change content
 */
SemanticType FunctionData_Returns_GetSemantic(void *data, int index);

/**
 * @brief Destroy function for Function data
 * @param data Data
 */
void FunctionData_Destroy(void *data);

#endif //!__FUNCTION_DATA_H__