#include <lapacke.h>
#include <stdio.h>
#include <stdlib.h>

#include "device/datasheet.h"
#include "stimulator/mna.h"
#include "util/errors.h"

int voltage_stimulation(
    network_state ns,
    connected_component cc,
    const interface it,
    double io[]
)
{
    // count the total number of sources, grounds, and loads
    int sources_count = 0;
    int loads_count = 0;
    int grounds_count = 0;

    for (int i = 0; i < cc.ws_count; i++)
    {
        sources_count += it.sources_mask[cc.ws_skip + i];
        grounds_count += it.grounds_mask[cc.ws_skip + i];
        loads_count += it.loads_mask[cc.ws_skip + i];
    }

    // calculate size of MNA matrix
    int size = cc.ws_count + sources_count - grounds_count;

    // count and memorize the number of grounds and sources preceding a node
    int skips[cc.ws_count] = { };
    int index[cc.ws_count] = { };

    for (int i = 1; i < cc.ws_count; i++)
    {
        // count the grounds and sources according to the preceding
        skips[i] = skips[i - 1] + it.grounds_mask[cc.ws_skip + i - 1];
        index[i] = index[i - 1] + it.sources_mask[cc.ws_skip + i - 1];
    }

    // create data structures to perform the analysis
    double A[size * size] = { };
    double b[size] = { };

    // create a support vector used by LAPACK routine
    // if pivots[k] > 0, then rows and columns k and pivots[k] were interchanged
    int pivots[size];

    // create a support vector used by LAPACK routine
    // if INFO = 0, WORK(1) returns the optimal L-WORK
    double work[size];

    // fill the A matrix according to the junctions values
    #pragma omp parallel for
    for (int k = 0; k < cc.js_count; k++)
    {
        int i = cc.Is[k] / cc.ws_count;
        int j = cc.Is[k] % cc.ws_count;

        bool is_i_ground = it.grounds_mask[cc.ws_skip + i];
        bool is_j_ground = it.grounds_mask[cc.ws_skip + j];

        if (! is_i_ground)
        {
            // sum the outgoing conductances on the diagonal
            // linearization of A[i - skips[i]][i - skips[i]]
            #pragma omp atomic
            A[(i - skips[i]) * size + i - skips[i]] += ns.Ys[cc.js_skip + k];
        }

        if (! is_j_ground)
        {
            #pragma omp atomic
            A[(j - skips[j]) * size + j - skips[j]] += ns.Ys[cc.js_skip + k];
        }

        if (! is_i_ground && ! is_j_ground)
        {
            // negate the conductance of the cc.Ys matrix
            // linearization of A[i - skips[i]][j - skips[j]] and A[j - skips[j]][i - skips[i]]
            // Since LAPACK only considers the upper / lower triangular matrix,
            // setting just one of the two is enough
            A[(i - skips[i]) * size + j - skips[j]] = - ns.Ys[cc.js_skip + k];
        }
    }

    // fill the A matrix according to the sources and loads
    #pragma omp parallel for
    for (int i = 0; i < cc.ws_count; i++)
    {
        // mark the sources with a 1 on the right and bottom most part of the matrix
        if (it.sources_mask[cc.ws_skip + i])
        {
            // linearization of:
            // A[size - it.sources_count + index[i]][i]
            // A[i][size - it.sources_count + index[i]]
            // Since LAPACK only considers the upper / lower triangular matrix,
            // setting just one of the two is enough
            A[(i - skips[i] + 1) * size - sources_count + index[i]] = 1;

            // if 'i' is a source, set its voltage value in b
            b[size - sources_count + index[i]] = io[cc.ws_skip + i];
        }

        // add the loads conductance to the diagonal of A
        if (it.loads_mask[cc.ws_skip + i])
        {
            // linearization of A[i - skips[i]][i - skips[i]]
            A[(i - skips[i]) * size + i - skips[i]] += it.loads_weight[cc.ws_skip + i];
        }
    }

    // consider A as an upper triangular matrix
    char L = 'L';

    // number of columns of the matrix b
    // and return values of the solution
    // e == 0: OK, e < 0: e has illegal value
    // e > 0: the diagonal of e is zero
    int b_size = 1, info;

    dsysv_rook_(&L, &size, &b_size, A, &size, pivots, b, &size, work, &size, &info, size);
    requires(info == 0, -1, "The MNA system cannot be solved! INFO = %d\n", info);

    // set the voltages in the ns.Vs array and the input currents in the io array
    // according to the MNA calculation
    #pragma opm parallel for
    for (int i = 0; i < cc.ws_count; i++)
    {
        int nsi = cc.ws_skip + i;

        if (it.sources_mask[nsi])
        {
            ns.Vs[nsi] = io[nsi];
            io[nsi] = - b[size - sources_count + index[i]];
        }
        else
        if (it.grounds_mask[nsi])
        {
            ns.Vs[nsi] = 0;
        }
        else
        {
            ns.Vs[nsi] = b[i - skips[i]];
        }
    }

    return 0;
}
