#include <math.h>
#include <stdio.h>

#include "nns.h"

int main()
{
    printf("Defining the Nanowire Network characteristics\n");

    // define the NN datasheet, i.e., its technical information
    const datasheet ds = {
        2000,
        40.0,
        14.0,
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

    printf("Serializing the Nanowire Network\n");

    // serialize the datasheet and network topology
    serialize_network(ds, nt, ".", 0);

    printf("Deserializing the Nanowire Network\n");

    // create the data structures to de-serialize the datasheet and network topology
    datasheet loaded_ds;
    network_topology loaded_nt;

    // deserialize the datasheet and network topology
    deserialize_network(&loaded_ds, &loaded_nt, ".", 0);

    printf("Construing the Nanowire Network equivalent electrical circuit\n");

    // interpret the NN as an electrical circuit
    const network_state ns = construe_circuit(ds, nt);

    printf("Serializing the network state\n");

    // serialize the network state
    serialize_state(ds, nt, ns, ".", 0, -1);

    printf("Deserializing the network state\n");

    // create a data-structure to deserialize the network state
    network_state loaded_ns;

    // deserialize the network state
    deserialize_state(ds, nt, &loaded_ns, ".", 0, -1);

    printf("Splitting the Nanowire Network in connected components\n");

    // separate the nanowire network in connected components
    connected_component* ccs = split_components(ds, nt, n2c, ccs_count);

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

    printf("Serializing the connected components\n");

    // serialize the connected components
    for (int i = 0; i < ccs_count; i++)
    {
        serialize_component(ccs[i], ".", 0, i);
    }

    printf("Deserializing the connected components\n");

    // create a data-structure to deserialize the connected components
    connected_component loaded_ccs[ccs_count];

    // deserialize the connected components
    for (int i = 0; i < ccs_count; i++)
    {
        deserialize_component(&loaded_ccs[i], ".", 0, i);
    }

    printf("Creating an interface to stimulate the nanowire network.\n");

    // creating the interface to stimulate the device
    int sources[1]   = { lcc.ws_skip };
    int grounds[1]   = { lcc.ws_skip + lcc.ws_count - 1 };
    int loads[1]     = { grounds[0] / 2 };
    double weight[1] = { 0.5 };

    interface it = {
        1, sources,
        1, grounds,
        1, loads, weight,
    };

    printf("Serializing the network interface\n");

    // serialize the network interface
    serialize_interface(it, ".", 0, 0);

    printf("Deserializing the network interface\n");

    // create a data-structure to deserialize the network interface
    interface loaded_it;

    // deserialize the network interface
    deserialize_interface(&loaded_it, ".", 0, 0);

    printf("Performing the voltage stimulation and weight update of the nanowire network\n");

    double ios[1];

    for (int i = 0; i < 100; i++)
    {
        update_conductance(ns, lcc);

        ios[0] = i / 20.0;

        voltage_stimulation(ns, lcc, it, ios);

        // serialize the state of the network state
        serialize_state(ds, nt, ns, ".", 0, i);
    }

    printf("Deserializing the last state of the network state\n");

    // create a data-structure to deserialize the state of the network state
    network_state loaded_lns;

    // deserialize the state of the network state
    deserialize_state(ds, nt, &loaded_lns, ".", 0, 0);

    printf("Freeing all the allocated memory\n");

    // free the network topology and state
    destroy_topology(nt);
    destroy_topology(loaded_nt);
    destroy_state(ns);
    destroy_state(loaded_ns);

    // note that it is not needed to free the first
    // interface as its arrays where allocated in the stack
    destroy_interface(loaded_it);

    // free the connected components data
    for (int i = 0; i < ccs_count; i++)
    {
        free(ccs[i].Is);
        free(loaded_ccs[i].Is);
    }
    free(ccs);

    printf("Terminating the simulation\n");

    return 0;
}
