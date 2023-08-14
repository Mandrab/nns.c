#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "device/network.h"

/// @brief Given an adjacency matrix, return a vector containing for each node
/// the index of the parent connected component.
/// 
/// @param[in] adj The adjacency matrix to explore.
/// @param[in] size The size of the adjacency matrix.
/// @return An array of length 'size' containing for each entry the index of
/// the parent connected component.
const int* map_components(bool** adj, int size);

/// @brief Differentiate the connected groups of wires as independent networks.
/// 
/// @param[in] ns The original, potentially disconnected, Nanowire Network.
/// @param[out] nss_count Will be set to the number of connected components
/// discovered.
/// @return The array of discovered connected components and corresponding
/// networks.
const network_state* connected_components(const network_state ns, int* nss_count);

/// @brief Given an array of connected components, select the largest.
/// 
/// @param[in] nss The array of connected components networks.
/// @param[in] count The number of connected components.
/// @return The pointer to the largest connected component.
const network_state* largest_component(const network_state* nss, int count);

#endif /* COMPONENTS_H */
