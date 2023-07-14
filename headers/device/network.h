#ifndef NETWORK_H
#define NETWORK_H

#include <stdbool.h>

#include "datasheet.h"
#include "junction.h"
#include "wire.h"

/// Topology of the Nanowire Network.
typedef struct
{
    wire*       Ws;         ///< Position of the wires in the packaging.
    int         js_count;   ///< Number of junctions identified.
    junction*   Js;         ///< Position of the identified junctions.
} network_topology;

typedef struct
{
    int     size;
    bool**  A;
    float** G;
    float** Y;
    float*  V;
} network_state;

network_topology create_network(datasheet ds);

network_state construe_circuit(datasheet ds, network_topology nt);

#endif /* NETWORK_H */
