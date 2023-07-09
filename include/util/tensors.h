#ifndef TENSORS_H
#define TENSORS_H

#include <stdbool.h>
#include <stdlib.h>

#define vector(TYPE, LENGTH) (TYPE*)malloc(LENGTH * sizeof(TYPE))

#define zeros_vector(TYPE, LENGTH) (TYPE*)calloc(LENGTH, sizeof(TYPE))

#define matrix(TYPE, WIDTH, HEIGHT)                                     \
    ({                                                                  \
        TYPE** result = (TYPE**)malloc(WIDTH * sizeof(TYPE*));          \
        for (int i = 0; i < WIDTH; i++)                                 \
        {                                                               \
            result[i] = vector(TYPE, HEIGHT);                           \
        }                                                               \
        result;                                                         \
    })

#define zeros_matrix(TYPE, WIDTH, HEIGHT)                               \
    ({                                                                  \
        TYPE** result = (TYPE**)malloc(WIDTH * sizeof(TYPE*));          \
        for (int i = 0; i < WIDTH; i++)                                 \
        {                                                               \
            result[i] = zvector(TYPE, HEIGHT);                          \
        }                                                               \
        result;                                                         \
    })

#define free_matrix(MATRIX, WIDTH)                                      \
    ({                                                                  \
        for (int i = 0; i < WIDTH; i++)                                 \
        {                                                               \
            free(MATRIX[i]);                                            \
        }                                                               \
        free(MATRIX);                                                   \
    })

float vdot(float* a, float* b, int size);

float* mdot(float** a, float** b, int size);

#endif /* TENSORS_H */
