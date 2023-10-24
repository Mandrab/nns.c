#include <stdio.h>
#include <stdlib.h>
#include <umfpack.h>

#include "device/datasheet.h"
#include "stimulator/mna.h"
#include "util/errors.h"

int voltage_stimulation(
    network_state ns,
    const connected_component cc,
    const interface it,
    double io[]
)
{
    // count the total number of sources connected to the CC
    int sources_count = 0;
    #pragma omp parallel for reduction(+ : sources_count)
    for (int i = cc.ws_skip; i < cc.ws_skip + cc.ws_count; i++)
    {
        sources_count += it.sources_mask[i];
    }

    // re-map a source index from the old matrix to the new one (right-most
    // side), and a node index from the old matrix to the new one
    int s2n[cc.ws_count];
    int n2n[cc.ws_count];

    for (int i = 0; i < cc.ws_count; i++)
    {
        // re-map nodes index according to the number of grounds preceding it
        n2n[i] = i ? 1 + n2n[i - 1] - it.grounds_mask[cc.ws_skip + i - 1] : 0;
    }

    // calculate the size of the MNA matrix considering the number of nodes
    // contained (n2n[-1] + 1), the possible grounding of the last node
    // (it.ground_mask[...]), and the number of sources
    int size = n2n[cc.ws_count - 1] + 1 - it.grounds_mask[cc.ws_skip + cc.ws_count - 1] + sources_count;

    // if i is a source, create a mapping to the right-most side of the matrix
    for (int i = 0, j = 0; i < cc.ws_count; i++)
    {
        if (it.sources_mask[cc.ws_skip + i])
        {
            s2n[i] = size - sources_count + j++;
        }
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
            A[n2n[i] * size + n2n[i]] += ns.Ys[cc.js_skip + k];
        }

        if (! is_j_ground)
        {
            // sum the outgoing conductances on the diagonal
            // linearization of A[n2n[j]][n2n[j]]
            A[n2n[j] * size + n2n[j]] += ns.Ys[cc.js_skip + k];
        }

        if (! is_i_ground && ! is_j_ground)
        {
            // negate the conductance of the cc.Ys matrix
            // linearization of A[n2n[i]][n2n[j]] and A[n2n[j]][n2n[i]]
            // Since LAPACK only considers the upper / lower triangular matrix,
            // setting just one of the two is enough
            A[n2n[i] * size + n2n[j]] = - ns.Ys[cc.js_skip + k];
            A[n2n[j] * size + n2n[i]] = - ns.Ys[cc.js_skip + k];
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
            A[(n2n[i] + 1) * s2n[i]] = 1;
            A[s2n[i] * size + n2n[i]] = 1;

            // if 'i' is a source, set its voltage value in b
            b[s2n[i]] = io[cc.ws_skip + i];
        }

        // add the loads conductance to the diagonal of A
        if (it.loads_mask[cc.ws_skip + i])
        {
            // linearization of A[n2n[i]][n2n[i]]
            A[n2n[i] * size + n2n[i]] += it.loads_weight[cc.ws_skip + i];
        }
    }

    // maximum possible size of the Ax/i arrays; it includes possible junctions
    // connected with the grounds (also if they shouldn't be count is not a
    // problem because the visit of Ai and Ax depends on Ap)
    int size_csr = 2 * cc.js_count + 2 * sources_count + cc.ws_count;

    // create the data structures to contain the sparse matrix in compressed
    // form: Ap is the pointer to the start of the next row, Ai is the column
    // of each entry Ax is the value of the junction
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
            io[nsi] = - x[s2n[i]];
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
