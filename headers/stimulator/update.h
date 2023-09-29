#ifndef UPDATE_H
#define UPDATE_H

#include "device/network.h"
#include "util/components.h"

/// @brief Update the weight of the nanowires junctions (i.e., their
/// conductance) according to the current stimulation (i.e., voltage
/// distribution in the network). The only weights updated are the ones
/// belonging to the Connected Component. As the `cc' references the data in
/// `ns', also this latter one is modified by the function. The original model
/// of this function was proposed by Enrique Miranda.
/// 
/// @param[in, out] ns The Nanowire Network electrical state that contains
/// the junctions to be updated.
/// @param[in, out] cc The Connected Component specifying the junctions to be
/// update.
void update_conductance(network_state ns, connected_component cc);

#endif /* UPDATE_H */
