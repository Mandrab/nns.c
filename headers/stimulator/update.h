#ifndef UPDATE_H
#define UPDATE_H

#include "device/network.h"

/// @brief Update the weight of the nanowires junctions (i.e., their conductance)
/// according to the current stimulation (i.e., voltage distribution in the network).
/// Original model proposed by Enrique Miranda.
/// 
/// @param[in, out] ns The Nanowire Network electrical state that will be updated.
void update_conductance(const network_state* ns);

#endif /* UPDATE_H */
