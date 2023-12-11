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
    // construct the weight and voltage arrays
    double* Y = vector(double, nt.js_count);
    double* V = zeros_vector(double, ds.wires_count);

    // fill the Ys array with the minimum conductance weight
    #pragma omp parallel for
    for (int i = 0; i < nt.js_count; i++)
    {
        Y[i] = Y_MIN;
    }

    // return the generated network state
    return (network_state){ Y, V };
}

int ntcmp(const void* e1, const void* e2)
{
    network_topology a = *((network_topology*)e1);
    network_topology b = *((network_topology*)e2);

    return a.js_count - b.js_count;
}

void destroy_topology(network_topology nt)
{
    free(nt.Ws);
    free(nt.Js);
}

void destroy_state(network_state ns)
{
    free(ns.Ys);
    free(ns.Vs);
}
