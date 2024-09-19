/**
 * @file measures.h
 * 
 * @brief Defines the function to calculate the resistance distance between two
 * points in a Nanowire Network Connected Component.
 * 
 * This file contains the declaration of the function `resistive_distance`
 * which computes the equivalent resistance between two nanowires in a
 * connected component of a Nanowire Network. The measure is based on the
 * circuital state of the network.
 * 
 * @note If the two nanowires are not connected, the function returns -1 as an
 * error value.
 */
#ifndef MEASURES_H
#define MEASURES_H

#include "device/network.h"
#include "device/component.h"

/// @brief Calculate the resistance distance between two points of a Nanowire
/// Network Connected Component.
/// 
/// @param[in] ns The Nanowire Network circuital state on which perform the
/// measure.
/// @param[in] cc The Connected Component of the Nanowire Network on which
/// perform the measure.
/// @param[in] a The index of the first nanowire to consider for the
/// calculation (relative to the Connected Component).
/// @param[in] b The index of the second nanowire to consider for the
/// calculation (relative to the Connected Component).
/// @return The resistance of the Nanowire Network equivalent circuit between
/// the two points. -1 if an error occurs (e.g. if the two nanowires are not
/// connected).
double resistive_distance(
    const network_state ns,
    const connected_component cc,
    int a,
    int b
);

#endif /* MEASURES_H */
