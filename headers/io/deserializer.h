#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include "device/network.h"
#include "interface/interface.h"

/// @brief Deserialize the static characteristics of the network from a file
/// named "nanowire_network.ID.dat", where ID is the univocal identifier of the
/// NN.
/// 
/// @param[out] ds The datasheet to set according to the data saved in the
/// file. It has to not be initialized.
/// @param[out] nt The network topology to set according to the data saved in
/// the file. It has to not be initialized.
/// @param[out] mapping The mapping of each nanowire to the parent connected
/// component.
/// @param[in] id The univocal id of the network that will determine its file
/// name.
/// @return -1 if the file cannot be open or if its version is not supported,
/// 0 otherwise.
int deserialize_network(datasheet* ds, network_topology* nt, int** mapping, int id);

/// @brief Deserialize the state of the network from a file named
/// "nanowire_state.ID.dat", where ID is the univocal identifier of the NN.
/// 
/// @param[out] ns The network state to set according to the data saved in the
/// file. It has to not be initialized.
/// @param[in] id The univocal id of the network that will determine its file
/// name.
/// @param[in] step The id of the network state in a specific instant.
/// @return -1 if the file cannot be open or if its version is not supported,
/// 0 otherwise.
int deserialize_state(network_state* ns, int id, int step);

/// @brief Deserialize the interface to the network from a file named
/// "interface.ID.dat", where ID is the univocal identifier of the NN.
/// 
/// @param[out] it The interface to set according to the data saved in the
/// file. It has to not be initialized.
/// @param[in] id The univocal id of the network that will determine its file
/// name.
/// @param[in] step The id of the network interface in a specific instant.
/// @return -1 if the file cannot be open or if its version is not supported,
/// 0 otherwise.
int deserialize_interface(interface* ns, int id, int step);

#endif /* DESERIALIZER_H */
