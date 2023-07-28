#ifndef TENSORS_H
#define TENSORS_H

#include <stdbool.h>
#include <stdlib.h>

/// @brief Allocate a vector with the specified number of elements of the
/// specified type.
/// 
/// @param[in] TYPE The type of the array to allocate.
/// @param[in] MACRO_LENGTH The number of elements contained in the array.
/// @return The allocated array.
#define vector(TYPE, MACRO_LENGTH)                                          \
    ({                                                                      \
        TYPE* VECTOR_RESULT = (TYPE*)malloc((MACRO_LENGTH) * sizeof(TYPE)); \
        if (VECTOR_RESULT == NULL)                                          \
        {                                                                   \
            exit(EXIT_FAILURE);                                             \
        }                                                                   \
        VECTOR_RESULT;                                                      \
    })

/// @brief Allocate a vector with the specified number of elements and with
/// the specified type and initialize it to 0.
/// 
/// @param[in] TYPE The type of the array to allocate.
/// @param[in] MACRO_LENGTH The number of elements contained in the array.
/// @return The allocated and zero-initialized array.
#define zeros_vector(TYPE, MACRO_LENGTH)                                    \
    ({                                                                      \
        TYPE* VECTOR_RESULT = (TYPE*)calloc((MACRO_LENGTH), sizeof(TYPE));  \
        if (VECTOR_RESULT == NULL)                                          \
        {                                                                   \
            exit(EXIT_FAILURE);                                             \
        }                                                                   \
        VECTOR_RESULT;                                                      \
    })

/// @brief Allocate a matrix of the specified type with the specified width
/// and height.
/// 
/// @param[in] TYPE The type of the matrix to allocate.
/// @param[in] MACRO_WIDTH The width of the matrix, i.e. size of the array of
/// pointers.
/// @param[in] MACRO_HEIGHT The height of the matrix, i.e. size of the arrays
/// of values.
/// @return The allocated matrix.
#define matrix(TYPE, MACRO_WIDTH, MACRO_HEIGHT)                             \
    ({                                                                      \
        TYPE** MATRIX_RESULT = vector(TYPE*, (MACRO_WIDTH));                \
        for (int MACRO_i = 0; MACRO_i < (MACRO_WIDTH); MACRO_i++)           \
        {                                                                   \
            MATRIX_RESULT[MACRO_i] = vector(TYPE, (MACRO_HEIGHT));          \
        }                                                                   \
        MATRIX_RESULT;                                                      \
    })

/// @brief Allocate a matrix of the specified type with the specified width
/// and height and initialize it to zero.
/// 
/// @param[in] TYPE The type of the matrix to allocate.
/// @param[in] MACRO_WIDTH The width of the matrix, i.e. size of the array of
/// pointers.
/// @param[in] MACRO_HEIGHT The height of the matrix, i.e. size of the arrays
/// of values.
/// @return The allocated and zero-initialized matrix.
#define zeros_matrix(TYPE, MACRO_WIDTH, MACRO_HEIGHT)                       \
    ({                                                                      \
        TYPE** MATRIX_RESULT = zeros_vector(TYPE*, (MACRO_WIDTH));          \
        for (int MACRO_i = 0; MACRO_i < (MACRO_WIDTH); MACRO_i++)           \
        {                                                                   \
            MATRIX_RESULT[MACRO_i] = zeros_vector(TYPE, (MACRO_HEIGHT));    \
        }                                                                   \
        MATRIX_RESULT;                                                      \
    })

/// @brief Free a matrix of the given width, including its sub-arrays.
/// 
/// @param[in] MATRIX The matrix to free.
/// @param[in] MACRO_WIDTH The width of the matrix, i.e. size of the array of
/// pointers.
#define free_matrix(MATRIX, MACRO_WIDTH)                                    \
    ({                                                                      \
        for (int MACRO_i = 0; MACRO_i < (MACRO_WIDTH); MACRO_i++)           \
        {                                                                   \
            free(MATRIX[MACRO_i]);                                          \
        }                                                                   \
        free(MATRIX);                                                       \
    })

#endif /* TENSORS_H */
