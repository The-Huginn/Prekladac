/**
 * @file data.h
 * @author Rastislav Budinsky
 * @brief This file contains interface for working with data of ID / FUNCTION
 */
#ifndef __DATA_H__
#define __DATA_H__

#include "semantictype.h"
#include "symbolelement.h"
#include "vector.h"

/**
 * @param dataType SymbolType
 * @return void* 
 */
void *Data_Init(SymbolType dataType);

/**
 * @brief Should be called only upon Variable SymbolType
 * @param data Data
 * @param semanticType Semantic type to be changed to
 */
void Data_Variable_SetSemantic(void *data, SemanticType semanticType);

/**
 * @brief Should be called only upon Function SymbolType
 * @param data Data
 * @param semanticType SemanticType
 */
void Data_AddSemanticReturn(void *data, SemanticType semanticType);

/**
 * @brief Should be called only upon Function SymbolType
 * @param data Data
 * @param dataType SymbolType
 * @param semanticType SemanticType
 */
void Data_AddSemanticParam(void *data, SemanticType semanticType);

/**
 * @brief Based on dataType returns Semantic type of ID, in case of FUNCTION Semantic type of first return value of VOID if none
 * @param data Data
 * @param dataType SymbolType
 * @return Semantic type of data
 */
SemanticType Data_GetSemanticType(void *data, SymbolType dataType);

/**
 * @brief Should be called only upon Function SymbolType
 * @param data Data
 * @return pointer to the Vector of SemanticType* of parameters. @note Do not change content
 */
Vector *Data_Function_Params(void *data);

/**
 * @brief Should be called only upon Function SymbolType
 * @param data Data
 * @return pointer to the Vector of SemanticType* of return values. @note Do not change content
 */
Vector *Data_Function_ReturnVals(void *data);

/**
 * @param data Data
 * @param dataType SymbolType
 * @return previously assigned ID or -1
 */
int Data_GetID(void *data, SymbolType dataType);

/**
 * @brief assigns new ID
 * @param data Data
 * @param dataType SymbolType
 * @param new_ID newly assigned ID
 */
void Data_SetID(void *data, SymbolType dataType, int new_ID);

/**
 * @brief Destroy function for Function data
 * @param data Data
 */
void Data_Function_Destroy(void *data);

/**
 * @brief Destroy function for Id data
 * @param data Data
 */
void Data_Variable_Destroy(void *data);

#endif //!__DATA_H__