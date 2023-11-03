#include <stdio.h>
#include <stdlib.h>
#include <umfpack.h>

#include "device/datasheet.h"
#include "stimulator/mna.h"
#include "util/errors.h"

// Useful links:
// UMFPACK: https://users.encs.concordia.ca/~krzyzak/R%20Code-Communications%20in%20Statistics%20and%20Simulation%202014/Zubeh%F6r/SuiteSparse/UMFPACK/Doc/QuickStart.pdf
// CSR representation: https://people.sc.fsu.edu/~jburkardt/data/cc/cc.html
// CSR representation: https://www.youtube.com/watch?v=a2LXVFmGH_Q
int voltage_stimulation(
    network_state ns,
    const connected_component cc,
    const interface it,
    double io[]
)
{
    // describe the nanowire connection type (default none), and create an
    // array containing the weight of a possible load connected to a nanowire
    connection_t nct[cc.ws_count] = { };
    double ws[cc.ws_count];

    // count the total number of sources connected to the CC, and mark the
    // nanowire as a source for a faster access
    int sources_count = 0;
    for (int i = 0; i < it.sources_count; i++)
    {
        int nwi = it.sources_index[i] - cc.ws_skip;
        if (0 <= nwi && nwi < cc.ws_count)
        {
            nct[nwi] = SOURCE;
            sources_count++;
        }
    }

    // mark the nanowire as a ground for a faster access
    for (int i = 0; i < it.grounds_count; i++)
    {
        int nwi = it.grounds_index[i] - cc.ws_skip;
        if (0 <= nwi && nwi < cc.ws_count)
        {
            nct[nwi] = GROUND;
        }
    }

    // mark the nanowire as a load for a faster access and save its weight
    for (int i = 0; i < it.loads_count; i++)
    {
        int nwi = it.loads_index[i] - cc.ws_skip;
        if (0 <= nwi && nwi < cc.ws_count)
        {
            nct[nwi] = LOAD;
            ws[nwi] = it.loads_weight[i];
        }
    }

    // re-map the index of a source marker from the old matrix to the new one
    // (right-most side), and a node index from the old matrix to the new one
    int s2n[cc.ws_count];
    int n2n[cc.ws_count];

    for (int i = 0; i < cc.ws_count; i++)
    {
        // re-map nodes index according to the number of grounds preceding it
        n2n[i] = i ? 1 + n2n[i - 1] - (nct[i - 1] == GROUND) : 0;
    }

    // calculate the size of the MNA matrix considering the number of nodes
    // contained (n2n[-1] + 1), the possible grounding of the last node
    // (it.ground_mask[...]), and the number of sources
    int size = n2n[cc.ws_count - 1] + 1 - (nct[cc.ws_count - 1] == GROUND) + sources_count;

    // if i is a source, create a mapping to the right-most side of the matrix
    for (int i = 0, j = 0; i < cc.ws_count; i++)
    {
        if (nct[i] == SOURCE)
        {
            s2n[i] = size - sources_count + j++;
        }
    }

    // lp[*] contains the lengths of the column pointer by the pointer Ap[*]
    // ds[*] contains the index of the diagonal element of row/column '*'
    int lp[cc.ws_count] = { };
    int ds[cc.ws_count];

    // initialize ds with a negative number
    memset(ds, 0xff, cc.ws_count * sizeof(int));

    // count the junctions in each row and set the index of the diagonal element
    for (int k = 0; k < cc.js_count; k++)
    {
        int i = cc.Is[k] / cc.ws_count;
        int j = cc.Is[k] % cc.ws_count;

        ds[i] = ds[i] < 0 ? lp[i] : ds[i];

        lp[i] += nct[j] != GROUND;
        lp[j] += nct[i] != GROUND;
    }

    // set the index of the diagonals not set in the previous cycle (it is
    // possible that a row j has junctions only from i | i < j; in this case
    // the diagonal is the last element of the row: i.e., lp[j])
    for (int i = 0; i < cc.ws_count; i++)
    {
        ds[i] = ds[i] < 0 ? lp[i] : ds[i];
    }

    // create the data structures to contain the sparse matrix in compressed
    // form: Ap is the pointer to the start of the next row
    int Ap[size + 1] = { };

    // add the diagonal and the source markers to the count of the non-zero
    // elements in Ai/Ax (ie, lp), and calculate the starting index of the rows
    for (int i = 0; i < cc.ws_count; i++)
    {
        // the Ap array does not include grounds, so skip them
        if (nct[i] == GROUND)
        {
            continue;
        }

        // sum the diagonal and the possible source marker for the node
        lp[i] += nct[i] == SOURCE ? 2 : 1;

        // set the starting point of the row in Ai/Ax and increase
        // the last element of Ap (needed for the CSR matrix form)
        Ap[n2n[i]] = Ap[size];
        Ap[size] += lp[i];
    }

    // add the bottom source-rows to the Ap count
    for (int i = size - sources_count; i < size; i++)
    {
        Ap[i] = Ap[size]++;
    }

    // create the data structures to contain the sparse matrix in compressed
    // form: Ai is the column of each entry, Ax is the value of each junction;
    // create the array x to contain the solution of the equations system;
    // create an array containing for each row the number of memorized elements
    double Ax[Ap[size]] = { }, x[size];
    int Ai[Ap[size]], me[cc.ws_count] = { };

    // fill the CSR matrix by iterating on each nanowire network
    for (int i = 0, k = 0; i < cc.ws_count; i++)
    {
        // check all the junctions of the nanowire 'i'
        while (k < cc.js_count && i == cc.Is[k] / cc.ws_count)
        {
            // get the index of the incident nanowire 'j'
            int j = cc.Is[k] % cc.ws_count;

            if (nct[i] != GROUND)
            {
                // sum in the 'i' diagonal the value of the junction conductance
                Ai[Ap[n2n[i]] + ds[i]] = n2n[i];
                Ax[Ap[n2n[i]] + ds[i]] += ns.Ys[cc.js_skip + k];

                // if the number of the memorized elements is equal to the
                // index of the diagonal increase the first to avoid overwrites
                if (me[i] == ds[i])
                {
                    me[i]++;
                }

                // save the negated junction value and increase the me counter
                if (nct[j] != GROUND)
                {
                    Ai[Ap[n2n[i]] + me[i]] = n2n[j];
                    Ax[Ap[n2n[i]] + me[i]] = - ns.Ys[cc.js_skip + k];
                    me[i]++;
                }
            }

            if (nct[j] != GROUND)
            {
                // sum in the 'i' diagonal the value of the junction conductance
                Ai[Ap[n2n[j]] + ds[j]] = n2n[j];
                Ax[Ap[n2n[j]] + ds[j]] += ns.Ys[cc.js_skip + k];

                // if the number of the memorized elements is equal to the
                // index of the diagonal increase the first to avoid overwrites
                if (me[j] == ds[j])
                {
                    me[j]++;
                }

                // save the negated junction value and increase the me counter
                if (nct[i] != GROUND)
                {
                    Ai[Ap[n2n[j]] + me[j]] = n2n[i];
                    Ax[Ap[n2n[j]] + me[j]] = - ns.Ys[cc.js_skip + k];
                    me[j]++;
                }
            }

            // check the new junction
            k++;
        }

        if (nct[i] == SOURCE)
        {
            // set the source marker in the rightmost part of the matrix
            Ai[Ap[n2n[i]] + lp[i] - 1] = s2n[i];
            Ax[Ap[n2n[i]] + lp[i] - 1] = 1;

            // set the source marker in the bottom part of the matrix
            Ai[Ap[s2n[i]]] = n2n[i];
            Ax[Ap[s2n[i]]] = 1;
        }

        if (nct[i] == LOAD)
        {
            // add the load weight to the row diagonal
            Ax[Ap[n2n[i]] + ds[i]] += ws[i];
        }
    }

    // create the b array to contain the solution of the equation
    // system, and set it according to the values in the io array
    double b[size] = { };
    for (int i = 0; i < it.sources_count; i++)
    {
        int nwi = it.sources_index[i] - cc.ws_skip;
        if (0 <= nwi && nwi < cc.ws_count)
        {
            b[s2n[nwi]] = io[i];
        }
    }

    // create an array to retrieve the information about the sys. eq. solution
    double info[UMFPACK_INFO];

    // perform a column pre-ordering to reduce fill-in and a symbolic factorization
    void* Symbolic;
    umfpack_di_symbolic(size, size, Ap, Ai, Ax, &Symbolic, NULL, info);
    requires(info[UMFPACK_STATUS] == UMFPACK_OK, -1, "Columns contain row indices in increasing order / with duplicates! The MNA system cannot be solved! INFO = %f\n", info[UMFPACK_STATUS]);

    // perform the numerical factorization, PAQ=LU, PRAQ=LU, or P(R\A)Q=LU
    void* Numeric;
    umfpack_di_numeric(Ap, Ai, Ax, Symbolic, &Numeric, NULL, info);
    requires(info[UMFPACK_STATUS] == UMFPACK_OK, -1, "Numeric factorization was unsuccessful! The MNA system cannot be solved! INFO = %f\n", info[UMFPACK_STATUS]);

    // solve a linear system for the solution X
    umfpack_di_solve(UMFPACK_A, Ap, Ai, Ax, x, b, Numeric, NULL, info);
    requires(info[UMFPACK_STATUS] == UMFPACK_OK, -1, "The MNA system cannot be solved! INFO = %f\n", info[UMFPACK_STATUS]);

    // deallocate the Symbolic and Numeric objects
    umfpack_di_free_symbolic(&Symbolic);
    umfpack_di_free_numeric(&Numeric);

    // set the voltages in the ns.Vs array and the input currents
    // in the io array according to the MNA calculation
    #pragma omp parallel for
    for (int i = 0; i < cc.ws_count; i++)
    {
        int nsi = cc.ws_skip + i;

        if (nct[i] == SOURCE)
        {
            // the values will be set in a later cycle
            continue;
        }
        else
        if (nct[i] == GROUND)
        {
            ns.Vs[nsi] = 0;
        }
        else
        {
            ns.Vs[nsi] = x[n2n[i]];
        }
    }

    // set the value of the source nodes in the voltage array,
    // and the intensity of the drawn current in the io array
    for (int i = 0; i < it.sources_count; i++)
    {
        ns.Vs[it.sources_index[i]] = io[i];

        int nwi = it.sources_index[i] - cc.ws_skip;
        if (0 <= nwi && nwi < cc.ws_count)
        {
            io[i] = - x[s2n[nwi]];
        }
    }

    return 0;
}

int voltage_stimulation_mea(
    network_state ns,
    const connected_component cc,
    const MEA mea,
    double io[]
)
{
    interface it = mea2interface(mea);
    int result = voltage_stimulation(ns, cc, it, io);
    destroy_interface(it);
    return result;
}
