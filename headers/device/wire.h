/**
 * @file wire.h
 * 
 * @brief Defines the `wire' structure and a function for comparing wires by
 * length.
 * 
 * This file contains the definition of the `wire` structure, which represents
 * a nanowire with its centroid, start / end positions, and length. It also
 * provides a comparison function `wcmp` to compare two nanowires based on
 * their lengths.
 */
#ifndef WIRE_H
#define WIRE_H

#include "util/point.h"

/// @brief Stores the starting and ending positions of a wire
/// together with its centroid position and total length.
typedef struct
{
    point   centroid;     ///< Position of the centroid of the wire.
    point   start_edge;   ///< Position of one of the edges of the wire.
    point   end_edge;     ///< Position of one of the edges of the wire.
    double  length;       ///< Total length of the wire.
} wire;

/// @brief Compares two nanowires according to their length.
/// 
/// @param[in] e1 Pointer to the first element to compare.
/// @param[in] e2 Pointer to the second element to compare.
/// @return 0 if the nanowires have the same length; < 0 if e1 is shorter than
/// e2; > 0 otherwise.
int wcmp(const void* e1, const void* e2);

#endif /* WIRE_H */
