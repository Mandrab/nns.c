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
/// values of the junctions, their position in the linearized adjacency matrix,
/// and the voltage distribution in the network nanowires.
typedef struct
{
    int*        Is;         ///< Linearized index of the junctions (i.e.,
                            ///< junction(i, j) = junction(i * size + j)).
                            ///< Ordered to specify the position of the
                            ///< Ys(i, j) weight.
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

/// @brief Destroy a network topology memorized in the stack. This function
/// practically frees the wires and junctions arrays of the network topology.
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
