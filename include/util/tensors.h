#ifndef TENSORS_H
#define TENSORS_H

#include <stdbool.h>
#include <stdlib.h>

#define vector(TYPE, LENGTH)                                            \
    ({                                                                  \
        TYPE* VECTOR_RESULT = (TYPE*)malloc(LENGTH * sizeof(TYPE));     \
        if (VECTOR_RESULT == NULL)                                      \
        {                                                               \
            exit(EXIT_FAILURE);                                         \
        }                                                               \
        VECTOR_RESULT;                                                  \
    })

#define zeros_vector(TYPE, LENGTH)                                      \
    ({                                                                  \
        TYPE* VECTOR_RESULT = (TYPE*)calloc(LENGTH, sizeof(TYPE));      \
        if (VECTOR_RESULT == NULL)                                      \
        {                                                               \
            exit(EXIT_FAILURE);                                         \
        }                                                               \
        VECTOR_RESULT;                                                  \
    })

#define matrix(TYPE, WIDTH, HEIGHT)                                     \
    ({                                                                  \
        TYPE** MATRIX_RESULT = vector(TYPE*, WIDTH);                    \
        for (int MACRO_i = 0; MACRO_i < WIDTH; MACRO_i++)               \
        {                                                               \
            MATRIX_RESULT[MACRO_i] = vector(TYPE, HEIGHT);              \
        }                                                               \
        MATRIX_RESULT;                                                  \
    })

#define zeros_matrix(TYPE, WIDTH, HEIGHT)                               \
    ({                                                                  \
        TYPE** MATRIX_RESULT = zeros_vector(TYPE*, WIDTH);              \
        for (int MACRO_i = 0; MACRO_i < WIDTH; MACRO_i++)               \
        {                                                               \
            MATRIX_RESULT[MACRO_i] = zeros_vector(TYPE, HEIGHT);        \
        }                                                               \
        MATRIX_RESULT;                                                  \
    })

#define free_matrix(MATRIX, WIDTH)                                      \
    ({                                                                  \
        for (int MACRO_i = 0; MACRO_i < WIDTH; MACRO_i++)               \
        {                                                               \
            free(MATRIX[MACRO_i]);                                      \
        }                                                               \
        free(MATRIX);                                                   \
    })

float vdot(float* a, float* b, int size);

float* vprod(float a, float* v, int size);

float* mdot(float** a, float** b, int size);

float** mprod(float a, float** v, int size);

#endif /* TENSORS_H */
