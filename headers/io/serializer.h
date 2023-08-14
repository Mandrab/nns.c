#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "device/network.h"
#include "interface/interface.h"

/// @brief Serialize the static characteristics of the Nanowire Network to a
/// file named "nanowire_network.ID.dat", where ID is the univocal identifier
/// of the NN. Any other file named the same will be overwritten.
/// 
/// @param[in] ds The datasheet to serialize.
/// @param[in] nt The network topology to serialize.
/// @param[in] mapping The mapping of each node to the parent connected
/// component.
/// @param[in] id The univocal id of the network that will determine its file
/// name.
/// @return -1 if the file cannot be open, 0 otherwise.
int serialize_network(const datasheet ds, const network_topology nt, const int* mapping, int id);

/// @brief Serialize the network state (a.k.a. electrical equivalent circuit)
/// to a file named "nanowire_state.ID.dat", where ID is the univocal
/// identifier of the NN. Any other file named the same will be overwritten.
/// 
/// @param[in] ns The network state to serialize.
/// @param[in] id The univocal id of the network that will determine its file
/// name.
/// @param[in] step The id of the network state in a specific instant.
/// @return -1 if the file cannot be open, 0 otherwise.
int serialize_state(const network_state ns, int id, int step);

/// @brief Serialize the network interface to a file named "interface.ID.dat",
/// where ID is the univocal identifier of the NN. Any other file named the
/// same will be overwritten.
/// 
/// @param[in] it The interface to serialize.
/// @param[in] id The univocal id of the network that will determine its file
/// name.
/// @param[in] step The id of the network interface in a specific instant.
/// @return -1 if the file cannot be open, 0 otherwise.
int serialize_interface(const interface it, int id, int step);

#endif /* SERIALIZER_H */
