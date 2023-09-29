#ifndef NETWORK_H
#define NETWORK_H

#include "datasheet.h"
#include "junction.h"
#include "wire.h"

/// @brief Topology of the Nanowire Network.
typedef struct
{
    wire*       Ws;         ///< Position of the wires in the package.
    int         js_count;   ///< Number of junctions identified.
    junction*   Js;         ///< Position of the identified junctions and index
                            ///< of the corresponding wires. Sorted according
                            ///< to the first and second wires index.
} network_topology;

/// @brief State of the Nanowire Network.
typedef struct
{
    int*        Is;         ///< Linearized index of the junctions (i.e.,
                            ///< junction(i, j) = junction(i * size + j)).
                            ///< Ordered to specify the position of the
                            ///< Ys(i, j) weight.
    double*     Ys;         ///< Admittances of the equivalent electrical
                            ///< circuit. If the Nanowire Network has been
                            ///< divided in connected-components, it is ordered
                            ///< according to the index of the parent connected
                            ///< component. Otherwise, they are ordered
                            ///< according to the index of the first and second
                            ///< wire. 
    double*     Vs;         ///< Voltage distribution across the wires.
} network_state;

/// @brief Create the Nanowire Network topology determining the wires and
/// junctions distribution.
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
void destroy_topology(network_topology nt);

/// @brief Destroy a network state memorized in the stack. This function
/// practically frees Is, Ys, and Vs of the network state. The arrays to be
/// freed must have been allocated in the heap with malloc/calloc.
/// 
/// @param[in, out] ns The network state to destroy.
void destroy_state(network_state ns);

#endif /* NETWORK_H */
