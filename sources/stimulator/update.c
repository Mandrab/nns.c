#include <math.h>
#include <stdlib.h>

#include "stimulator/update.h"
#include "config.h"

void update_conductance(network_state* ns)
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

            // calculate the conductance of the junction
            double g = (ns->Y[i][j] - Y_MIN) / (Y_MAX - Y_MIN);
            g = kp / kpd * (1 + kd / kp * g * exp(-TAU * kpd));

            // calculate and set circuit admittance
            ns->Y[i][j] = ns->Y[j][i] = Y_MIN + g * (Y_MAX - Y_MIN);
        }
    }
}
