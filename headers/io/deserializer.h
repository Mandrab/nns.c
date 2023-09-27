#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include "device/network.h"
#include "interface/interface.h"

/// @brief Deserialize the static characteristics of the network from a file
/// named "nanowire_network.ID.dat", where ID is the univocal identifier of the
/// NN. If any problem occurs, the system will exit with an error.
/// 
/// @param[out] ds The datasheet to set according to the data saved in the
/// file. It has to not be initialized.
/// @param[out] nt The network topology to set according to the data saved in
/// the file. It has to not be initialized.
/// @param[in] path The base path containing the /device_ID folder.
/// @param[in] id The univocal id of the network that will determine its file
/// name.
void deserialize_network(datasheet* ds, network_topology* nt, char* path, int id);

/// @brief Deserialize the state of the network from a file named
/// "nanowire_state.ID.dat", where ID is the univocal identifier of the NN. If
/// any problem occurs, the system will exit with an error.
/// 
/// @param[out] ns The network state to set according to the data saved in the
/// file. It has to not be initialized.
/// @param[in] path The base path containing the /device_ID folder.
/// @param[in] id The univocal id of the network that will determine its file
/// name.
/// @param[in] step The id of the network state in a specific instant.
void deserialize_state(network_state* ns, char* path, int id, int step);

/// @brief Deserialize the interface to the network from a file named
/// "interface.ID.dat", where ID is the univocal identifier of the NN. If any
/// problem occurs, the system will exit with an error.
/// 
/// @param[out] it The interface to set according to the data saved in the
/// file. It has to not be initialized.
/// @param[in] path The base path containing the /device_ID folder.
/// @param[in] id The univocal id of the network that will determine its file
/// name.
/// @param[in] step The id of the network interface in a specific instant.
void deserialize_interface(interface* ns, char* path, int id, int step);

#endif /* DESERIALIZER_H */
