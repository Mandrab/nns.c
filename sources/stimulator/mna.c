#include <stdio.h>
#include <stdlib.h>
#include <umfpack.h>

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

    // int grounded_junctions = 0;
    // for (int k = 0; k < cc.js_count; k++)
    // {
    //     int i = cc.Is[k] / cc.ws_count;
    //     int j = cc.Is[k] % cc.ws_count;

    //     grounded_junctions += it.grounds_mask[cc.ws_skip + i]
    //         + it.grounds_mask[cc.ws_skip + j];
    // }

    // calculate size of MNA matrix
    int size = cc.ws_count + sources_count - grounds_count;
    int size_csr = 0;// 2 * cc.js_count + 2 * sources_count + cc.ws_count - grounded_junctions;

    // count and memorize the number of grounds and sources preceding a node

    int skips[cc.ws_count] = { };
    int index[cc.ws_count] = { };
    int n2n[cc.ws_count] = { };

    for (int i = 1; i < cc.ws_count; i++)
    {
        // count the grounds and sources according to the preceding
        skips[i] = skips[i - 1] + it.grounds_mask[cc.ws_skip + i - 1];
        index[i] = index[i - 1] + it.sources_mask[cc.ws_skip + i - 1];
        n2n[i] = i - skips[i];
    }

    // create data structures to perform the analysis
    double A[size * size] = { };
    double b[size] = { };

    // fill the A matrix according to the junctions values
    for (int k = 0; k < cc.js_count; k++)
    {
        int i = cc.Is[k] / cc.ws_count;
        int j = cc.Is[k] % cc.ws_count;

        bool is_i_ground = it.grounds_mask[cc.ws_skip + i];
        bool is_j_ground = it.grounds_mask[cc.ws_skip + j];

        if (! is_i_ground)
        {
            // sum the outgoing conductances on the diagonal
            // linearization of A[n2n[i]][n2n[i]]
            if (A[n2n[i] * size + n2n[i]] < 0.001)
                size_csr++;
            A[n2n[i] * size + n2n[i]] += ns.Ys[cc.js_skip + k];
        }

        if (! is_j_ground)
        {
            if (A[n2n[j] * size + n2n[j]] < 0.001)
                size_csr++;
            A[n2n[j] * size + n2n[j]] += ns.Ys[cc.js_skip + k];
        }

        if (! is_i_ground && ! is_j_ground)
        {
            // negate the conductance of the cc.Ys matrix
            // linearization of A[n2n[i]][n2n[j]] and A[n2n[j]][n2n[i]]
            // Since LAPACK only considers the upper / lower triangular matrix,
            // setting just one of the two is enough
            A[n2n[i] * size + n2n[j]] = - ns.Ys[cc.js_skip + k];
            size_csr++;
            A[n2n[j] * size + n2n[i]] = - ns.Ys[cc.js_skip + k];
            size_csr++;
        }
    }

    // fill the A matrix according to the sources and loads
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
            A[(n2n[i] + 1) * size - sources_count + index[i]] = 1;
            size_csr++;
            A[(size - sources_count + index[i]) * size + n2n[i]] = 1;
            size_csr++;

            // if 'i' is a source, set its voltage value in b
            b[size - sources_count + index[i]] = io[cc.ws_skip + i];
        }

        // add the loads conductance to the diagonal of A
        if (it.loads_mask[cc.ws_skip + i])
        {
            if (A[n2n[i] * size + n2n[i]] < 0.001)
                size_csr++;
            // linearization of A[n2n[i]][n2n[i]]
            A[n2n[i] * size + n2n[i]] += it.loads_weight[cc.ws_skip + i];
        }
    }

    // https://users.encs.concordia.ca/~krzyzak/R%20Code-Communications%20in%20Statistics%20and%20Simulation%202014/Zubeh%F6r/SuiteSparse/UMFPACK/Doc/QuickStart.pdf
    // https://people.sc.fsu.edu/~jburkardt/data/cc/cc.html
    int Ap[size + 1], Ai[size_csr];
    double Ax[size_csr], x[size];

    for (int i = 0, c = 0; i < size; i++)
    {
        Ap[i] = c;
        for (int j = 0; j < size; j++)
        {
            if (A[i * size + j] != 0)
            {
                Ai[c] = j;
                Ax[c] = A[i * size + j];

                Ap[size] = ++c;
            }
        }
    }

    double info[UMFPACK_INFO];

    void* Symbolic;
    umfpack_di_symbolic(size, size, Ap, Ai, Ax, &Symbolic, NULL, info);
    requires(info[UMFPACK_STATUS] == UMFPACK_OK, -1, "Columns contain row indices in increasing order / with duplicates! The MNA system cannot be solved! INFO = %d\n", info[UMFPACK_STATUS]);

    void* Numeric;
    umfpack_di_numeric(Ap, Ai, Ax, Symbolic, &Numeric, NULL, info);
    requires(info[UMFPACK_STATUS] == UMFPACK_OK, -1, "Numeric factorization was unsuccessful! The MNA system cannot be solved! INFO = %d\n", info[UMFPACK_STATUS]);

    umfpack_di_solve(UMFPACK_A, Ap, Ai, Ax, x, b, Numeric, NULL, info);
    requires(info[UMFPACK_STATUS] == UMFPACK_OK, -1, "The MNA system cannot be solved! INFO = %d\n", info[UMFPACK_STATUS]);

    umfpack_di_free_symbolic(&Symbolic);
    umfpack_di_free_numeric(&Numeric);

    // set the voltages in the ns.Vs array and the input currents in the io array
    // according to the MNA calculation
    #pragma opm parallel for
    for (int i = 0; i < cc.ws_count; i++)
    {
        int nsi = cc.ws_skip + i;

        if (it.sources_mask[nsi])
        {
            ns.Vs[nsi] = io[nsi];
            io[nsi] = - x[size - sources_count + index[i]];
        }
        else
        if (it.grounds_mask[nsi])
        {
            ns.Vs[nsi] = 0;
        }
        else
        {
            ns.Vs[nsi] = x[n2n[i]];
        }
    }

    return 0;
}
