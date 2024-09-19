/**
 * @file update.h
 * 
 * @brief Defines the `update_conductance` function.
 * 
 * This file provides the declaration of the `update_conductance` function, 
 * which updates the conductance (or weights) of the nanowire junctions in 
 * a network according to the current voltage stimulation. The update is 
 * applied only to junctions belonging to the Connected Component (CC) of the
 * network, and modifies both the network's electrical state and the connected
 * component data structures.
 * 
 * The underlying model for this function was originally proposed by Enrique
 * Miranda.
 */
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
