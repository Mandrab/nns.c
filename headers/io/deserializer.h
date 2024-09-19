/**
 * @file deserializer.h
 * 
 * @brief Defines deserialization functions for all the data structures of the
 * library.
 * 
 * This file contains declarations for functions used to deserialize various
 * components of a nanowire network from files. These components include the
 * network's datasheet, topology, state, connected components, interface, and
 * MEA (Multi-Electrode Array). The deserialization process involves reading
 * data from files named according to specific conventions, which include
 * unique identifiers.
 * 
 * @note If any error occurs during deserialization, the program will exit with
 * an error.
 */
#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include "device/network.h"
#include "device/component.h"
#include "interface/interface.h"
#include "interface/mea.h"

/// @brief Deserialize the static characteristics of the network from a file
/// named "nanowire_network.ID.dat", where ID is the univocal identifier of the
/// NN. If any problem occurs, the program will exit with an error.
/// 
/// @param[out] ds The datasheet to set according to the data saved in the
/// file. It has to not be initialized.
/// @param[out] nt The network topology to set according to the data saved in
/// the file. It has to not be initialized.
/// @param[in] path The base path containing the /device_ID folder.
/// @param[in] id The univocal id of the network that will determine its file
/// name.
void deserialize_network(
    datasheet* ds,
    network_topology* nt,
    char* path,
    int id
);

/// @brief Deserialize the state of the network from a file named
/// "nanowire_state.ID.dat", where ID is the univocal identifier of the NN. If
/// any problem occurs, the program will exit with an error.
/// 
/// @param[in] ds The datasheet of the nanowire network to deserialize.
/// @param[in] nt The topology of the nanowire network to deserialize.
/// @param[out] ns The network state to set according to the data saved in the
/// file. It has to not be initialized.
/// @param[in] path The base path containing the /device_ID folder.
/// @param[in] id The univocal id of the network that will determine its file
/// name.
/// @param[in] step The id of the network state in a specific instant.
void deserialize_state(
    const datasheet ds,
    const network_topology nt,
    network_state* ns,
    char* path,
    int id,
    int step
);

/// @brief Deserialize a network connected component. NN_ID is the univocal
/// identifier of the NN. If any problem occurs, the program will exit with an
/// error.
/// 
/// @param cc The connected component to deserialize.
/// @param path The base path containing the /device_ID folder.
/// @param nn_id  The univocal id of the network that will determine its folder
/// name.
/// @param cc_id The index of the connected component.
void deserialize_component(
    connected_component* cc,
    char* path,
    int nn_id,
    int cc_id
);

/// @brief Deserialize the interface to the network from a file named
/// "it_STEP.dat", where ID is the univocal identifier of the NN and STEP is
/// the instant in which the interface was saved. If any problem occurs, the
/// program will exit with an error.
/// 
/// @param[out] it The interface to set according to the data saved in the
/// file. It has to not be initialized.
/// @param[in] path The base path containing the /device_ID folder.
/// @param[in] id The univocal id of the network that will determine its file
/// name.
/// @param[in] step The id of the network interface in a specific instant.
void deserialize_interface(interface* it, char* path, int id, int step);

/// @brief Deserialize the MEA of the network from a file named "mea_STEP.nns"
/// where ID is the univocal identifier of the NN and STEP is the instant in
/// which the MEA was saved. If any problem occurs, the program will exit with
/// an error.
/// 
/// @param[out] it The interface to set according to the data saved in the
/// file. It has to not be initialized.
/// @param[in] path The base path containing the /device_ID folder.
/// @param[in] id The univocal id of the network that will determine its file
/// name.
/// @param[in] step The id of the network MEA in a specific instant.
void deserialize_mea(MEA* mea, char* path, int id, int step);

#endif /* DESERIALIZER_H */
