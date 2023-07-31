#include <lapacke.h>
#include <stdio.h>
#include <stdlib.h>

#include "device/datasheet.h"
#include "stimulator/mna.h"
#include "util/errors.h"

void voltage_stimulation(network_state ns, const interface it, const double* inputs)
{
    // calculate size of MNA matrix
    int size = ns.size + it.sources_count - it.grounds_count;

    // create data structures to perform the analysis
    double A[size * size] = { };
    double b[size] = { };

    // create a support vector used by LAPACK routine
    // if pivots[k] > 0, then rows and columns k and pivots[k] were interchanged
    int pivots[size];

    // create a support vector used by LAPACK routine
    // if INFO = 0, WORK(1) returns the optimal L-WORK
    double work[size];

    // count and memorize the number of grounds preceding a node
    int skips[size] = { };
    for (int i = 1; i < size; i++)
    {
        if (it.grounds_mask[i - 1])
        {
            skips[i] = skips[i - 1] + 1;
        }
        else
        {
            skips[i] = skips[i - 1];
        }
    }

    // fill the A matrix as required to run the MNA algorithm 
    #pragma omp parallel for
    for (int i = 0; i < ns.size; i++)
    {
        // if i is a grounded node continue
        if (it.grounds_mask[i])
        {
            continue;
        }

        // iterate over the columns of A (excluding grounds)
        // setting the value as the negate of the conductance
        // and summing their positive values on the diagonal
        #pragma omp parallel for
        for (int j = 0; j < ns.size; j++)
        {
            // if i == j, continue to the next element (the diagonal is 0)
            if (i == j)
            {
                continue;
            }

            // sum the outgoing conductances on the diagonal
            // linearization of A[i - skips[i]][i - skips[i]]
            A[(i - skips[i]) * size + i - skips[i]] += ns.Y[i][j];

            // if j is a grounded node continue
            if (it.grounds_mask[j])
            {
                continue;
            }

            // negate the conductance of the Y matrix
            // linearization of A[i - skips[i]][j - skips[j]] and A[j - skips[j]][i - skips[i]]
            A[(i - skips[i]) * size + j - skips[j]] = - ns.Y[i][j];
            A[(j - skips[j]) * size + i - skips[i]] = - ns.Y[i][j]; // TODO not useful as LAPACK considers the upper or lower triangular matrix
        }

        // add the loads conductance to the diagonal of A
        if (it.loads_mask[i])
        {
            // linearization of A[i - skips[i]][i - skips[i]]
            A[(i - skips[i]) * size + i - skips[i]] += it.loads_weight[i];
        }
    }

    // set the sources on the right and bottom of the matrix
    for (int i = 0; i < it.sources_count; i++)
    {
        // set the source on the right-most side of the matrix
        // linearization of A[i][size - it.sources_count + i]
        A[
            i * size +                              // row
            size - it.sources_count + i             // column
        ] = 1;

        // set the source on the bottom side of the matrix
        // linearization of A[size - it.sources_count + i][i]
        A[
            size * (i + size - it.sources_count) +  // row
            i                                       // column
        ] = 1;  // TODO not useful as LAPACK considers the upper or lower triangular matrix
    }

    // fill the b vector with the known voltages
    for (int i = 0; i < it.sources_count; i++)
    {
        b[size - it.sources_count + i] = inputs[i];
    }

    // consider A as an upper triangular matrix
    char L = 'L';

    // number of columns of the matrix b
    // and return values of the solution
    // e == 0: OK, e < 0: e has illegal value
    // e > 0: the diagonal of e is zero
    int b_size = 1, info;

    dsysv_(&L, &size, &b_size, A, &size, pivots, b, &size, work, &size, &info, size);
    assert(info == 0, info, "The MNA system cannot be solved!");

    // set the voltages vector
    #pragma opm parallel for
    for (int i = 0; i < ns.size; i++)
    {
        if (it.grounds_mask[i])
        {
            ns.V[i] = 0;
        }
        else
        {
            ns.V[i] = b[i - skips[i]];
        }
    }
}
