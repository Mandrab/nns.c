#include <math.h>

#include "stimulator/update.h"
#include "config.h"

void update_conductance(network_state ns, connected_component cc)
{
    // iterate over all the junction indexes
    #pragma omp parallel for
    for (int k = 0; k < cc.js_count; k++)
    {
        // get the index of the wires in the nanowire-network state
        int i = cc.ws_skip + cc.Is[k] / cc.ws_count;
        int j = cc.ws_skip + cc.Is[k] % cc.ws_count;

        // calculate the delta voltage on each junction
        double ΔV = fabs(ns.Vs[i] - ns.Vs[j]);

        // compute the potentiation and depression coefficients
        double kp = KP * exp(ETA_P * ΔV);
        double kd = KD * exp(-ETA_D * ΔV);
        double kpd = kp + kd;

        // calculate the conductance of the junction
        double g = (ns.Ys[cc.js_skip + k] - Y_MIN) / (Y_MAX - Y_MIN);
        g = kp / kpd * (1 + kd / kp * g * exp(-TAU * kpd));

        // calculate and set circuit admittance
        ns.Ys[cc.js_skip + k] = Y_MIN + g * (Y_MAX - Y_MIN);
    }
}
