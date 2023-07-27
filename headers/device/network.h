#ifndef NETWORK_H
#define NETWORK_H

#include <stdbool.h>

#include "datasheet.h"
#include "junction.h"
#include "wire.h"

/// @brief Topology of the Nanowire Network.
typedef struct
{
    wire*       Ws;             ///< Position of the wires in the packaging.
    int         js_count;       ///< Number of junctions identified.
    junction*   Js;             ///< Position of the identified junctions.
} network_topology;

/// @brief State of the Nanowire Network.
typedef struct
{
    int         size;           /// Number of wires in the network.
    bool**      A;              /// Adjacency matrix of the Nanowire Network.
    double**    G;              /// Conductances of the equivalent electrical circuit.
    double**    Y;              /// Admittances of the equivalent electrical circuit.
    double*     V;              /// Voltage distribution across the wires.
} network_state;


/// @brief Create the Nanowire Network topology determining the wires and junctions distribution.
/// 
/// @param[in] ds The datasheet describing the Nanowire Network.
/// @return The topology of the created Nanowire Network.
const network_topology create_network(const datasheet ds);

/// @brief Construe the equivalent electrical circuit of the Nanowire Network.
/// 
/// @param[in] ds The datasheet describing the Nanowire Network.
/// @param[in] nt The topology of the Nanowire Network.
/// @return The electrical representation of the Nanowire Network.
const network_state construe_circuit(const datasheet ds, const network_topology nt);

#endif /* NETWORK_H */
