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
    double**    Y;              /// Admittances of the equivalent electrical circuit.
    double*     V;              /// Voltage distribution across the wires.
} network_state;


/// @brief Create the Nanowire Network topology determining the wires and junctions distribution.
/// 
/// @param[in] ds The datasheet describing the Nanowire Network.
/// @return The topology of the created Nanowire Network.
network_topology create_network(const datasheet ds);

/// @brief Construe the equivalent electrical circuit of the Nanowire Network.
/// 
/// @param[in] ds The datasheet describing the Nanowire Network.
/// @param[in] nt The topology of the Nanowire Network.
/// @return The electrical representation of the Nanowire Network.
network_state construe_circuit(const datasheet ds, const network_topology nt);

/// @brief Destroy a network topology memorized in the stack. This function
/// practically frees the wires and junctions array of the network topology.
/// The arrays to be freed must have been allocated in the heap with
/// malloc/calloc.
/// 
/// @param[in, out] nt The network topology to destroy.
void destroy_stack_topology(network_topology nt);

/// @brief Destroy a network topology memorized in the stack. This function
/// practically frees the wires and junctions array of the network topology. In
/// addition it frees also the network_topology pointer. The arrays to be freed
/// must have been allocated in the heap with malloc/calloc.
/// 
/// @param[in, out] nt The pointer to the network topology to destroy.
void destroy_heap_topology(network_topology* nt);

/// @brief Destroy a network state memorized in the stack. This function
/// practically frees A, Y, and V of the network state. The matrixes and arrays
/// to be freed must have been allocated in the heap with malloc/calloc.
/// 
/// @param[in, out] ns The network state to destroy.
void destroy_stack_state(network_state ns);

/// @brief Destroy a network state memorized in the stack. This function
/// practically frees A, Y, and V of the network state. In addition it frees
/// also the network_state pointer. The matrixes and arrays to be freed must
/// have been allocated in the heap with malloc/calloc.
/// 
/// @param[in, out] ns The pointer to the network state to destroy.
void destroy_heap_state(network_state* ns);

#endif /* NETWORK_H */
