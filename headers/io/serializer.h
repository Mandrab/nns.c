#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "device/network.h"
#include "device/component.h"
#include "interface/interface.h"
#include "interface/mea.h"

/// @brief Serialize the static characteristics of the Nanowire Network to a
/// file named "nn.nns" in a folder named "device_ID", where ID is the univocal
/// identifier of the NN. Any other file named the same will be overwritten. If
/// any problem occurs, the program will exit with an error.
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
/// to a file named "ns_STEP.nns" in a folder named "device_ID", where ID is
/// the univocal identifier of the NN and STEP is the snapshot instant. Any
/// other file named the same will be overwritten. If any problem occurs, the
/// program will exit with an error.
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

/// @brief Serialize a connected component of a nanowire network to a file
/// named "cc_CC_ID.nns" in a folder named "device_ID", where NN_ID is the
/// univocal identifier of the NN and CC_ID is the univocal identified of the
/// CC. Any other file named the same will be overwritten. If any problem
/// occurs, the program will exit with an error.
/// 
/// @param[in] cc The connected component to serialize.
/// @param[in] path The base path in which put the /device_ID folder.
/// @param[in] nn_id The univocal id of the network that will determine its
/// file name.
/// @param[in] cc_id The index of the connected component.
void serialize_component(
    const connected_component cc,
    char* path,
    int nn_id,
    int cc_id
);

/// @brief Serialize the network interface to a file named "it_STEP.nns" in a
/// folder named "device_ID", where ID is the univocal identifier of the NN and
/// STEP is the instant in which the interface was saved. Any other file named
/// the same will be overwritten. If any problem occurs, the program will exit
/// with an error.
/// 
/// @param[in] it The interface to serialize.
/// @param[in] path The base path in which put the /device_ID folder.
/// @param[in] id The univocal id of the network that will determine its file
/// name.
/// @param[in] step The id of the network interface in a specific instant.
void serialize_interface(const interface it, char* path, int id, int step);

/// @brief Serialize the network MEA to a file named "mea_STEP.nns" in a
/// folder named "device_ID", where ID is the univocal identifier of the NN and
/// STEP is the instant in which the MEA was saved. Any other file named the
/// same will be overwritten. If any problem occurs, the program will exit with
/// an error.
/// 
/// @param[in] mea The MEA to serialize.
/// @param[in] path The base path in which put the /device_ID folder.
/// @param[in] id The univocal id of the network that will determine its file
/// name.
/// @param[in] step The id of the network MEA in a specific instant.
void serialize_mea(const MEA mea, char* path, int id, int step);

#endif /* SERIALIZER_H */
