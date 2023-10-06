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

    // create an array to map each node index with
    // the index of its parent connected_component
    // create a counter to count the number of CCs
    int n2c[ds.wires_count], ccs_count;

    // generate the NN topology
    const network_topology nt = create_network(ds, n2c, &ccs_count);

    printf("Construing the Nanowire Network equivalent electrical circuit\n");

    // interpret the NN as an electrical circuit
    const network_state ns = construe_circuit(ds, nt);

    printf("Splitting the Nanowire Network in connected components\n");

    // separate the nanowire network in connected components
    connected_component* ccs = split_components(ds, nt, ns, n2c, ccs_count);

    printf("Finding and selecting the largest connected component of the Nanowire Network\n");

    // select the largest connected component among all
    connected_component lcc = ccs[0];
    for (int i = 0; i < ccs_count; i++)
    {
        if (ccs[i].ws_count > lcc.ws_count)
        {
            lcc = ccs[i];
        }
    }

    printf("Selected a connected component composed of %d nanowires over a total of %d\n", lcc.ws_count, ds.wires_count);

    printf("Creating an interface to stimulate the nanowire network.\n");

    // creating the interface to stimulate the device
    bool sources[ds.wires_count] = { };
    sources[lcc.ws_skip] = true;
    bool grounds[ds.wires_count] = { };
    bool loads[ds.wires_count] = { };
    loads[lcc.ws_skip + lcc.ws_count - 1] = true;
    double loads_weight[ds.wires_count] = { };
    loads_weight[lcc.ws_skip + lcc.ws_count - 1] = 0.001;

    interface it = {
        ds.wires_count,
        1, sources,
        0, grounds,
        1, loads, loads_weight,
    };
    double v[ds.wires_count];

    printf("Performing the voltage stimulation and weight update of the nanowire network\n");

    // let the system stabilize
    for (int i = 0; i < 1000; i++)
    {
        update_conductance(ns, lcc);
    }

    // update and stimulate the nanowire network
    printf("Conductance variation: ");
    for (int i = 0; i < 100; i++)
    {
        update_conductance(ns, lcc);

        v[lcc.ws_skip] = 5.00;

        voltage_stimulation(ns, lcc, it, v);

        printf("%f ", fabs(v[lcc.ws_skip] / 5.0));
    }

    // stop stimulating the nanowire network
    for (int i = 0; i < 100; i++)
    {
        update_conductance(ns, lcc);

        v[lcc.ws_skip] = 0.0;

        voltage_stimulation(ns, lcc, it, v);

        printf("%f ", fabs(v[lcc.ws_skip] / 5.0));
    }
    printf("\n");

    printf("Freeing all the allocated memory\n");

    // free the network topology and state
    destroy_topology(nt);
    destroy_state(ns);

    // free the connected components data
    for (int i = 0; i < ccs_count; i++)
    {
        free(ccs[i].Is);
    }

    printf("Terminating the simulation\n");

    return 0;
}
