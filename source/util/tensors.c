#include "util/tensors.h"

// Compute the dot product of two vectors
float vdot(float* a, float* b, int size)
{
    float result = 0.0;

    #pragma omp parallel for reduction(+:result)
    for (int i = 0; i < size; i++)
    {
        result += a[i] * b[i];
    }

    return result;
}

// Compute the dot product of two matrixes
float* mdot(float** a, float** b, int size)
{
    float* result = vector(float, size);

    #pragma omp parallel for
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            result[i] += a[i][j] * b[j][i];
        }
    }

    return result;
}
