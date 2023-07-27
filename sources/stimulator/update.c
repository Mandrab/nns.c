#include <math.h>
#include <stdlib.h>

#include "stimulator/update.h"
#include "config.h"

void update_conductance(const network_state* ns)
{
    // iterate over all the junction indexes
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < ns->size; i++)
    {
        for (int j = i + 1; j < ns->size; j++)
        {
            if (! ns->A[i][j])
            {
                continue;
            }

            // calculate the delta voltage on each junction
            double ΔV = fabs(ns->V[i] - ns->V[j]);

            // compute the potentiation and depression coefficients
            double kp = KP * exp(ETA_P * ΔV);
            double kd = KD * exp(-ETA_D * ΔV);
            double kpd = kp + kd;

            // calculate and set the conductance
            double partial = kd / kp * ns->G[i][j] * exp(-TAU * kpd);
            ns->G[i][j] = ns->G[j][i] = kp / kpd * (1 + partial);

            // calculate and set circuit admittance
            ns->Y[i][j] = ns->Y[j][i] = Y_MIN + ns->G[i][j] * (Y_MAX - Y_MIN);
        }
    }
}
