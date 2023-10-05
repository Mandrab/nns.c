#include <stdlib.h>

#include "device/network.h"
#include "util/components.h"
#include "util/tensors.h"
#include "util/wires.h"
#include "config.h"

network_topology create_network(const datasheet ds, int n2c[], int* ccs_count)
{
    network_topology nt;

    // set the random seed for the device generation
    srand(ds.generation_seed);

    // generate the network wires distribution
    nt.Ws = drop_wires(ds);

    // detect junctions position
    detect_junctions(ds, nt.Ws, &nt.Js, &nt.js_count);

    // map each node index with the index of its parent connected_component
    *ccs_count = map_components(ds, nt, n2c);

    // sort according to connected components
    group_nanowires(ds, nt, n2c, *ccs_count);

    return nt;
}

network_state construe_circuit(const datasheet ds, const network_topology nt)
{
    // construct the index, weight and voltage arrays
    int* I = vector(int, nt.js_count);
    double* Y = vector(double, nt.js_count);
    double* V = zeros_vector(double, ds.wires_count);

    // fill the Is array with the junctions position, and
    // the Ys array with the minimum conductance weight
    #pragma omp parallel for
    for (int i = 0; i < nt.js_count; i++)
    {
        I[i] = nt.Js[i].first_wire * ds.wires_count + nt.Js[i].second_wire;
        Y[i] = Y_MIN;
    }

    // return the generated network state
    return (network_state){ I, Y, V };
}

void destroy_topology(network_topology nt)
{
    free(nt.Ws);
    free(nt.Js);
}

void destroy_state(network_state ns)
{
    free(ns.Is);
    free(ns.Ys);
    free(ns.Vs);
}
