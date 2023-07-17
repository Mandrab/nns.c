#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "device/network.h"

network_state* connected_components(network_state ns, int* nss_count, int** nss_nodes);

network_state* largest_component(network_state* nss, int count, int* nodes_count);

#endif /* COMPONENTS_H */
