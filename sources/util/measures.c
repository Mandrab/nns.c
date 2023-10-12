#include <lapacke.h>
#include <math.h>

#include "util/errors.h"
#include "util/measures.h"

double resistive_distance(
    const datasheet ds,
    const network_state ns,
    const connected_component cc,
    int a,
    int b
)
{
    // the resistance of a point is 0
    if (a == b)
    {
        return 0;
    }

    // create the data-structures used in the computation
    double S[cc.ws_count] = { };                // from SVD, the Σ
    double U[cc.ws_count * cc.ws_count] = { };  // from SVD, the U
    double VT[cc.ws_count * cc.ws_count] = { }; // from SVD, the V^T
    double Γ[cc.ws_count * cc.ws_count] = { };  // laplacian and pseudo-inverse
                                                // matrix
    double work[5 * cc.ws_count] = { };

    // create the laplacian (or admittance) matrix
    for (int k = 0; k < cc.js_count; k++)
    {
        int i = cc.Is[k] / cc.ws_count;
        int j = cc.Is[k] % cc.ws_count;

        Γ[i * cc.ws_count + i] += ns.Ys[cc.js_skip + k];
        Γ[j * cc.ws_count + j] += ns.Ys[cc.js_skip + k];
        Γ[i * cc.ws_count + j] -= ns.Ys[cc.js_skip + k];
        Γ[j * cc.ws_count + i] -= ns.Ys[cc.js_skip + k];
    }

    // decompose Γ by using SVD
    const char jobs = 'A';
    int info, lwork = 5 * cc.ws_count;
    dgesvd_(
        &jobs, &jobs,
        &cc.ws_count, &cc.ws_count, Γ, &cc.ws_count,
        S, U, &cc.ws_count, VT,
        &cc.ws_count, work, &lwork, &info, cc.ws_count, cc.ws_count
    );
    requires(info == 0, -1, "SVD computation failed! INFO = %d\n", info);

    // the calculation of the the point-to-point resistance involves
    // the computation of the Moore-Penrose pseudo-inverse, however
    // we consider (and thus compute) only the entries of the matrix
    // that are used in the formula: Γ[a][a] + Γ[b][b] - 2 * Γ[b][a].
    // Source: https://math.stackexchange.com/a/458415 for the Moore-
    // Penrose pseudo-inverse formula.
    for (int k = 0; k < cc.ws_count; k++)
    {
        double value = .0;
        if (fabs(S[k]) > 1e-12)
        {
            value = 1.0 / S[k];
        }

        // V * pseudo-inverse(S) * U^T -> V[i][k] * pseudo-inverse(S)[k] * U^T[k][j]
        // V[i][k] == VT[k * size + i], U^T[k][j] == U[j * size + k]
        Γ[a * cc.ws_count + a] += VT[a * cc.ws_count + k] * U[k * cc.ws_count + a] * value;
        Γ[b * cc.ws_count + b] += VT[b * cc.ws_count + k] * U[k * cc.ws_count + b] * value;
        Γ[b * cc.ws_count + a] += VT[a * cc.ws_count + k] * U[k * cc.ws_count + b] * value;
    }
    return Γ[a * cc.ws_count + a] + Γ[b * cc.ws_count + b] - 2 * Γ[b * cc.ws_count + a];
}
