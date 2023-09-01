#include <math.h>
#include <stdio.h>

#include "nns.h"

int main()
{
    printf("Defining the Nanowire Network characteristics\n");

    // define the NN datasheet, i.e., its technical information
    const datasheet ds = {
        2000,
        14.0,
        40.0,
        500,
        1234
    };

    printf("Creating the Nanowire Network\n");

    // generate the NN topology
    const network_topology nt = create_network(ds);

    printf("Construing the Nanowire Network equivalent electrical circuit\n");

    // interpret the NN as an electrical circuit
    const network_state ns = construe_circuit(ds, nt);

    printf("Finding and selecting the largest connected component of the Nanowire Network\n");

    // separate the network into connected component networks
    // and get the largest connected component state
    int nss_count;
    network_state* nss = connected_components(ns, &nss_count);
    network_state lns = largest_component(nss, nss_count);

    printf("Keeping the largest connected component (%d nodes) and dereferencing the others\n", lns.size);

    // free the unused connected components
    for (int i = 0; i < nss_count; i++)
    {
        if (nss[i].A != lns.A)
        {
            // nss is an array of nt, therefore we cannot free one of its elements, just its content
            destroy_stack_state(nss[i]);
        }
    }

    printf("Creating an interface to stimulate the nanowire network.\n");

    // creating the interface to stimulate the device
    bool sources[lns.size] = { };
    sources[0] = true;
    bool grounds[lns.size] = { };
    bool loads[lns.size] = { };
    loads[lns.size - 1] = true;
    double loads_weight[lns.size] = { };
    loads_weight[lns.size - 1] = 0.001;

    interface it = {
        lns.size,
        1, sources,
        0, grounds,
        1, loads, loads_weight,
    };
    double v[1] = { };

    printf("Performing the voltage stimulation and weight update of the nanowire network\n");

    // let the system stabilize
    for (int i = 0; i < 1000; i++)
    {
        update_conductance(lns);
    }

    // update and stimulate the nanowire network
    printf("Conductance variation: ");
    for (int i = 0; i < 100; i++)
    {
        update_conductance(lns);

        v[0] = 5.00;

        voltage_stimulation(lns, it, v);

        printf("%f ", fabs(v[0] / 5.0));
    }

    // stop stimulating the nanowire network
    for (int i = 0; i < 100; i++)
    {
        update_conductance(lns);

        v[0] = 0.0;

        voltage_stimulation(lns, it, v);

        printf("%f ", fabs(v[0] / 5.0));
    }
    printf("\n");

    printf("Freeing all the allocated memory\n");

    // free the network topology and state
    destroy_stack_topology(nt);
    destroy_stack_state(ns);

    // nss is an array of nt, therefore we cannot free one of its elements, just its content
    destroy_stack_state(lns);

    // lets free the array of nns
    free(nss);

    printf("Terminating the simulation\n");

    return 0;
}
