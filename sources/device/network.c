#include <stdlib.h>

#include "device/network.h"
#include "util/tensors.h"
#include "util/wires.h"
#include "config.h"

const network_topology create_network(const datasheet ds)
{
    // set the random seed for the device generation
    srand(ds.generation_seed);

    // generate the network wires distribution
    wire* wires = drop_wires(ds);

    // detect junctions position
    int junctions_count;
    junction* junctions;
    detect_junctions(ds, wires, &junctions, &junctions_count);

    return (network_topology){ wires, junctions_count, junctions };
}

const network_state construe_circuit(const datasheet ds, const network_topology nt)
{
    // obtain the adjacency matrix and create the Y matrix
    bool** adjacency_matrix = construe_adjacency_matrix(ds, nt);
    double** Y = zeros_matrix(double, ds.wires_count, ds.wires_count);

    // fill the Y matrix according to the adjacency matrix
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < ds.wires_count; i++)
    {
        for (int j = 0; j < ds.wires_count; j++)
        {
            Y[i][j] = adjacency_matrix[i][j] * Y_MIN;
        }
    }

    return (network_state){
        ds.wires_count,                                         // size
        adjacency_matrix,                                       // A
        zeros_matrix(double, ds.wires_count, ds.wires_count),   // G
        Y,                                                      // Y
        zeros_vector(double, ds.wires_count)                    // V
    };
}
