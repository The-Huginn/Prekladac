/**
 * @file vector.h
 * @author Rastislav Budinsky
 * @brief This file contains declaration of class vector and interface for using this class
 *      Vector is considered an self-expanding and self-shrinking array saved on heap
 *      @note functions do not check for NULL ptr being passed
 */
#include <stdbool.h>

typedef struct Vector_t Vector;

/**
 * @brief Creates Empty Vector
 * @param dtor Destructor for element
 * @return Vector pointer
 */
Vector *Vector_Init(void (*VectorElementDtor)(void*));

/**
 * @brief Clears and destroys Vector
 * @param vector Vector
 */
void Vector_Destroy(Vector *vector);

/**
 * @brief Clears Vector
 * @param vector Vector
 * @return true upon success otherwise false
 */
bool Vector_Clear(Vector *vector);

/**
 * @brief Adds data to the end of Vector
 * @param vector Vetor
 * @param data Data to be added
 */
bool Vector_PushBack(Vector *vector, void* data);

/**
 * @brief Destroys last element, on empty nothing
 * @param vector Vector
 * @return true upon success, otherwise false @note PopBack upon empty Vector is success too
 */
bool Vector_PopBack(Vector *vector);

/**
 * @param vector Vector
 * @return Last data in vector
 */
void *Vector_Back(Vector *vector);

/**
 * @param vector Vector
 * @return true if empty otherwise false
 */
bool Vector_IsEmpty(Vector *vector);

/**
 * @param vector Vector
 * @return Size of vector
 */
int Vector_Size(Vector *vector);

/**
 * @param vector Vector
 * @param index Index of wanted element
 * @return Element at certain index, NULL if out-of-bounds
 */
void* Vector_GetElement(Vector *vector, int index);

/**
 * @brief Inserts Element at specified index
 * @param vector Vector
 * @param index Where to add new data
 * @param data Data
 * @return true upon success otherwise false
 */
bool Vector_InsertElement(Vector *vector, int index, void* data);

/**
 * @param vector Vector
 * @param index Removes element on index position
 * @return true upon succes otherwise false, index out-of-bounds has no effect
 */
bool Vector_RemoveElement(Vector *vector, int index);
