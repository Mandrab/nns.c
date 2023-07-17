#include <math.h>
#include <stdlib.h>

#include "util/linalg.h"
#include "util/tensors.h"

void conjugate_gradient(float** A, float* x, float* b, int size)
{
    float r[size], p[size], Ap[size];

    // p = r = b - A * x
    #pragma omp parallel for
    for (int i = 0; i < size; i++)
    {
        float accumulator = 0.0;

        for (int j = 0; j < size; j++)
        {
            accumulator += A[i][j] * x[j];
        }

        p[i] = r[i] = b[i] - accumulator;
    }

    // calculate the residual error
    float ϵ = vdot(r, r, size);

    // iterate until the error is not negligible or we checked all the nodes
    for (int i = 0; sqrt(ϵ) > 1e-10 && i < size; i++)
    {
        // Ap = A * p
        #pragma omp parallel for
        for (int j = 0; j < size; j++)
        {
            float accumulator = 0.0;

            for (int k = 0; k < size; k++)
            {
                accumulator += A[j][k] * p[k];
            }

            Ap[j] = accumulator;
        }

        // calculate the update value
        float α = ϵ / vdot(p, Ap, size);

        #pragma omp parallel for
        for (int j = 0; j < size; j++)
        {
            // x = x + α * p
            x[j] += α * p[j];

            // r = r - α * Ap
            r[j] -= α * Ap[j];
        }

        // compute new residual and β
        float new_ϵ = vdot(r, r, size);
        float β = new_ϵ / ϵ;

        // update the residual
        ϵ = new_ϵ;

        // p = r + β * p
        #pragma omp parallel for
        for (int j = 0; j < size; j++)
        {
            p[j] = r[j] + β * p[j];
        }
    }
}
