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

    printf("Serializing the Nanowire Network\n");

    // serialize the datasheet and network topology
    serialize_network(ds, nt, ".", 0);

    printf("Deserializing the Nanowire Network\n");

    // create the data structures to de-serialize the datasheet and network topology
    datasheet loaded_ds;
    network_topology loaded_nt;

    // deserialize the datasheet and network topology
    deserialize_network(&loaded_ds, &loaded_nt, ".", 0);

    printf("Serializing the network state\n");

    // serialize the network state
    serialize_state(ns, ".", 0, -1);

    printf("Deserializing the network state\n");

    // create a data-structure to deserialize the network state
    network_state loaded_ns;

    // deserialize the network state
    deserialize_state(&loaded_ns, ".", 0, -1);

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
    grounds[lns.size - 1] = true;
    bool loads[lns.size] = { };
    loads[(int) lns.size / 2] = true;
    double weight[lns.size] = { };
    weight[(int) lns.size / 2] = 0.5;

    interface it = {
        lns.size,
        1, sources,
        1, grounds,
        0, loads, weight,
    };

    printf("Stimulate the nanowire network and serializing the state variation.\n");

    double v[1];

    for (int i = 0; i < 100; i++)
    {
        update_conductance(lns);

        v[0] = i / 20.0;

        voltage_stimulation(lns, it, v);

        // serialize the state of the largest connected component of the network
        serialize_state(lns, ".", 0, i);
    }

    printf("Deserializing the last state of the largest connected component\n");

    // create a data-structure to deserialize the state of the largest connected component of the network
    network_state loaded_lns;

    // deserialize the state of the largest connected component of the network
    deserialize_state(&loaded_lns, ".", 0, 0);

    printf("Serializing the network interface\n");

    // serialize the network interface
    serialize_interface(it, ".", 0, 0);

    printf("Deserializing the network interface\n");

    // create a data-structure to deserialize the network interface
    interface loaded_it;

    // deserialize the network interface
    deserialize_interface(&loaded_it, ".", 0, 0);

    printf("Freeing all the allocated memory\n");

    // free the network topology and state
    destroy_stack_topology(nt);
    destroy_stack_topology(loaded_nt);
    destroy_stack_state(ns);
    destroy_stack_state(loaded_ns);

    // nss is an array of nt, therefore we cannot free one of its elements, just its content
    destroy_stack_state(lns);
    destroy_stack_state(loaded_lns);

    // lets free the array of nns
    free(nss);

    // the content of the loaded interface is created with malloc and has to be freed
    destroy_stack_interface(loaded_it);

    printf("Terminating the simulation\n");

    return 0;
}
