#include <lapacke.h>
#include <math.h>

#include "util/errors.h"
#include "util/measures.h"

double resistive_distance(const network_state ns, int a, int b)
{
    // the resistance of a point is 0
    if (a == b)
    {
        return 0;
    }

    // create the data-structures used in the computation
    double S[ns.size] = { };            // from SVD, the Σ
    double U[ns.size * ns.size] = { };  // from SVD, the U
    double VT[ns.size * ns.size] = { }; // from SVD, the V^T
    double Γ[ns.size * ns.size] = { };  // laplacian and pseudo-inverse matrix
    double work[5 * ns.size] = { };

    // create the laplacian (or admittance) matrix
    for (int i = 0; i < ns.size; i++)
    {
        for (int j = 0; j < ns.size; j++)
        {
            if (i == j)
            {
                continue;
            }
            Γ[i * ns.size + i] += ns.Y[i][j];
            Γ[i * ns.size + j] -= ns.Y[i][j];
        }
    }

    // decompose Γ by using SVD
    char jobs = 'A';
    int info, lwork = 5 * ns.size;
    dgesvd_(&jobs, &jobs, &ns.size, &ns.size, Γ, &ns.size, S, U, &ns.size, VT, &ns.size, work, &lwork, &info, ns.size, ns.size);
    assert(info == 0, info, "SVD computation failed!\n");

    // the calculation of the the point-to-point resistance involves
    // the computation of the Moore-Penrose pseudo-inverse, however
    // we consider (and thus compute) only the entries of the matrix
    // that are used in the formula: Γ[a][a] + Γ[b][b] - 2 * Γ[b][a].
    // Source: https://math.stackexchange.com/a/458415 for the Moore-
    // Penrose pseudo-inverse formula.
    for (int k = 0; k < ns.size; k++)
    {
        double value = .0;
        if (fabs(S[k]) > 1e-12)
        {
            value = 1.0 / S[k];
        }

        // V * pseudo-inverse(S) * U^T -> V[i][k] * pseudo-inverse(S)[k] * U^T[k][j]
        // V[i][k] == VT[k * size + i], U^T[k][j] == U[j * size + k]
        Γ[a * ns.size + a] += VT[a * ns.size + k] * U[k * ns.size + a] * value;
        Γ[b * ns.size + b] += VT[b * ns.size + k] * U[k * ns.size + b] * value;
        Γ[b * ns.size + a] += VT[a * ns.size + k] * U[k * ns.size + b] * value;
    }
    return Γ[a * ns.size + a] + Γ[b * ns.size + b] - 2 * Γ[b * ns.size + a];
}
