#include <stdio.h>

#include "nns.h"

int main()
{
    printf("Defining the Nanowire Network characteristics\n");

    // define the NN datasheet, i.e., its technical information
    datasheet ds = {
        1500,
        14.0,
        14.0 * 0.35,
        500,
        1234
    };

    printf("Creating the Nanowire Network\n");

    // generate the NN topology
    network_topology nt = create_network(ds);

    printf("Construing the Nanowire Network equivalent electrical circuit\n");

    // interpret the NN as an electrical circuit
    network_state ns = construe_circuit(ds, nt);

    printf("Finding and selecting the largest connected component of the Nanowire Network\n");

    // separate the network into connected component networks
    // and get the largest connected component state
    int* nss_nodes, nss_count;
    network_state* nss = connected_components(ns, &nss_count, &nss_nodes);
    network_state* lns = largest_component(nss, nss_count, nss_nodes);

    printf("Creating an interface to stimulate the nanowire network.\n");

    // creating the interface to stimulate the device
    bool sources[lns->size] = { };
    sources[0] = true;
    bool grounds[lns->size] = { };
    grounds[lns->size - 1] = true;
    bool loads[lns->size] = { };

    interface it = {
        1, sources,
        1, grounds,
        0, loads, NULL,
    };
    float v[1] = { 5.0 };

    printf("Performing the voltage stimulation and weight update of the nanowire network\n");

    // update and stimulate the nanowire network
    for (int i = 0; i < 1000; i++)
    {
        update_conductance(lns);
        voltage_stimulation(*lns, it, v);
    }

    printf("Terminating the simulation\n");

    return 0;
}
