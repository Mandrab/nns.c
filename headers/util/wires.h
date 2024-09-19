/**
 * @file wires.h
 * 
 * @brief Contains an utilities to build a nanowire network. Not supposed to be
 * used directly by the user.
 */
#ifndef WIRES_H
#define WIRES_H

#include <stdbool.h>

#include "device/datasheet.h"
#include "device/junction.h"
#include "device/network.h"
#include "device/wire.h"

/// @brief Given the Nanowire Network description (i.e., the datasheet),
/// produce it by dropping the specified number of nanowires into the
/// package according to a normal distribution.
/// 
/// @param[in] The datasheet describing the Nanowire Network.
/// @return An array containing the information of the dropped wires.
wire* drop_wires(const datasheet ds);

/// @brief Detect the junctions between the nanowires composing the Nanowire
/// Network.
/// 
/// @param ds[in] The datasheet describing the Nanowire Network.
/// @param ws[in] An array containing the information of the dropped wires.
/// @param js[out] An uninitialized pointer to a vector of junctions to be
/// filled and returned.
/// @param js_count[out] The number of unique identified junctions. This means
/// that a junction between A and B will be present only once in the array.
void detect_junctions(const datasheet ds, const wire* ws, junction** js, int* js_count);

/// @brief Given the datasheet and the network topology, construe the Nanowire
/// Network adjacency matrix identifying the junctions between the nanowires.
/// 
/// @param ds[in] The datasheet describing the Nanowire Network.
/// @param nt[in] The topology of the Nanowire Network.
/// @return The adjacency matrix identifying the junctions between the wires.
bool** construe_adjacency_matrix(const datasheet ds, const network_topology nt);

#endif /* WIRES_H */
