#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "device/network.h"
#include "interface/interface.h"

/// @brief Serialize the static characteristics of the Nanowire Network to a
/// file named "nanowire_network.ID.dat", where ID is the univocal identifier
/// of the NN. Any other file named the same will be overwritten. If any
/// problem occurs, the system will exit with an error.
/// 
/// @param[in] ds The datasheet to serialize.
/// @param[in] nt The network topology to serialize.
/// @param[in] path The base path in which put the /device_ID folder.
/// @param[in] id The univocal id of the network that will determine its file
/// name.
void serialize_network(
    const datasheet ds,
    const network_topology nt,
    char* path,
    int id
);

/// @brief Serialize the network state (a.k.a. electrical equivalent circuit)
/// to a file named "nanowire_state.ID.dat", where ID is the univocal
/// identifier of the NN. Any other file named the same will be overwritten.
/// If any problem occurs, the system will exit with an error.
/// 
/// @param[in] ds The datasheet of the nanowire network to serialize.
/// @param[in] nt The topology of the nanowire network to serialize.
/// @param[in] ns The network state to serialize.
/// @param[in] path The base path in which put the /device_ID folder.
/// @param[in] id The univocal id of the network that will determine its file
/// name.
/// @param[in] step The id of the network state in a specific instant.
void serialize_state(
    const datasheet ds,
    const network_topology nt,
    const network_state ns,
    char* path,
    int id,
    int step
);

/// @brief Serialize the network interface to a file named "interface.ID.dat",
/// where ID is the univocal identifier of the NN. Any other file named the
/// same will be overwritten. If any problem occurs, the system will exit with
/// an error.
/// 
/// @param[in] it The interface to serialize.
/// @param[in] path The base path in which put the /device_ID folder.
/// @param[in] id The univocal id of the network that will determine its file
/// name.
/// @param[in] step The id of the network interface in a specific instant.
void serialize_interface(const interface it, char* path, int id, int step);

#endif /* SERIALIZER_H */
