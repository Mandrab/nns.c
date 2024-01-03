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

    printf("Construing the Nanowire Network equivalent electrical circuit\n");

    // interpret the NN as an electrical circuit
    const network_state ns = construe_circuit(ds, nt);

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

    printf("Connect a Micro-Electrode Array (MEA) to stimulate the Nanowire Network\n");

    // connect a MEA to the NN and identify the connection points with the nanowires
    MEA mea = connect_MEA(ds, nt);

    // set some electrodes as source, ground, and load
    mea.ct[0] = SOURCE;
    mea.ct[5] = LOAD;
    mea.ct[9] = GROUND;
    mea.ws[5] = 0.5;

    printf("Creating an interface to stimulate the nanowire network.\n");

    interface it = mea2interface(mea);

    printf("Performing the voltage stimulation and weight update of the nanowire network\n");

    double ios[1];

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

        ios[0] = 5.00;

        voltage_stimulation(ns, lcc, it, ios);

        printf("%f ", fabs(ios[0] / 5.0));
    }

    // stop stimulating the nanowire network
    for (int i = 0; i < 100; i++)
    {
        update_conductance(ns, lcc);

        ios[0] = 0.0;

        voltage_stimulation_mea(ns, lcc, mea, ios);

        printf("%f ", fabs(ios[0] / 5.0));
    }
    printf("\n");

    printf("Freeing all the allocated memory\n");

    // free the network topology and state
    destroy_topology(nt);
    destroy_state(ns);

    // destroy the created interface
    destroy_interface(it);

    // free the connected components data
    for (int i = 0; i < ccs_count; i++)
    {
        free(ccs[i].Is);
    }
    free(ccs);

    printf("Terminating the simulation\n");

    return 0;
}
