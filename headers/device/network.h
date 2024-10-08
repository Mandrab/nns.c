/**
 * @file network.h
 * 
 * @brief Defines the structures and functions used to model and simulate the
 * topology and electrical state of a Nanowire Network.
 * 
 * This file contains the necessary data structures and function declarations 
 * to represent a Nanowire Network, including the physical layout of nanowires 
 * and junctions, and the electrical properties such as voltage distribution 
 * and admittances. It also provides utilities for creating, copying, comparing, 
 * and destroying network topologies and states.
 * 
 * @note The structures here defined use dynamically allocated memory that must
 * be managed using the provided functions.
 */
#ifndef NETWORK_H
#define NETWORK_H

#include "datasheet.h"
#include "junction.h"
#include "wire.h"

/// @brief Topology of the Nanowire Network. It contains information about the
/// physical placement of nanowires and junctions in the package.
typedef struct
{
    wire*       Ws;         ///< Position of the nanowires in the package.
                            ///< Ordered according to parent connected
                            ///< components containing each nanowire and to
                            ///< their original index in the network. E.g.:
                            ///<    1,2,3   \in cc0
                            ///<    4,5     \in cc1
                            ///<    ws = [1,2,3,4,5]
    int         js_count;   ///< Number of junctions identified.
    junction*   Js;         ///< Position of the identified junctions and index
                            ///< of the corresponding wires. Sorted according
                            ///< to the first and second wires index. Since the
                            ///< nanowires are ordered according to their
                            ///< parent connected component, also the junctions
                            ///<  are ordered accordingly.
} network_topology;

/// @brief Electrical state of the Nanowire Network. It contains the varying
/// values of the junctions and the voltage distribution in the network
/// nanowires.
typedef struct
{
    double*     Ys;         ///< Admittances of the equivalent electrical
                            ///< circuit. Ordered according to the index of the
                            ///< parent connected component and to the index of
                            ///< their composing nanowires.
    double*     Vs;         ///< Voltage distribution across the nanowires.
} network_state;

/// @brief Create the Nanowire Network topology determining the wires and
/// junctions distribution. The function produces a network topology whose
/// nanowires and junctions are sorted according to their parent connected
/// component. Also returns the mapping between nodes index and parent
/// connected component index.
/// 
/// @param[in] ds The datasheet describing the Nanowire Network to realize.
/// @param[out] n2c The output mapping between nodes index and parent connected
/// component index.
/// @param[out] ccs_count The number of connected components discovered.
/// @return The topology of the created Nanowire Network.
network_topology create_network(const datasheet ds, int n2c[], int* ccs_count);

/// @brief Construe the equivalent electrical circuit of the Nanowire Network.
/// Both nanowires and junctions are ordered according to their parent
/// connected component.
/// 
/// @param[in] ds The datasheet describing the Nanowire Network.
/// @param[in] nt The topology of the Nanowire Network.
/// @return The electrical representation of the Nanowire Network.
network_state construe_circuit(const datasheet ds, const network_topology nt);

/// @brief Compare two network topologies according to the number of junctions.
/// 
/// @param[in] e1 Pointer to the first element to compare.
/// @param[in] e2 Pointer to the second element to compare.
/// @return 0 if the topologies contain the same amount of junctions; > 0 if e1
/// has more junctions than e2; < 0 otherwise.
int ntcmp(const void* e1, const void* e2);

/// @brief Create a deep copy of a network topology.
/// 
/// @param[in] ds The datasheet describing the network.
/// @param[in] nt The network topology to copy.
/// @return The deep copy of the network topology.
network_topology copy_topology(const datasheet ds, const network_topology nt);

/// @brief Create a deep copy of a network state.
/// 
/// @param[in] ds The datasheet describing the network.
/// @param[in] nt The topology of the network.
/// @param[in] ns The network state to copy.
/// @return The deep copy of the network state.
network_state copy_state(
    const datasheet ds,
    const network_topology nt,
    const network_state ns
);

/// @brief Destroy a network topology memorized in the stack. This function
/// practically frees the wires and junctions arrays of the network topology.
/// The arrays to be freed must have been allocated in the heap with
/// malloc/calloc.
/// 
/// @param[in, out] nt The network topology to destroy.
void destroy_topology(network_topology nt);

/// @brief Destroy a network state memorized in the stack. This function
/// practically frees Ys and Vs arrays of the network state. The arrays to be
/// freed must have been allocated in the heap with malloc/calloc.
/// 
/// @param[in, out] ns The network state to destroy.
void destroy_state(network_state ns);

#endif /* NETWORK_H */
