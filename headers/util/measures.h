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
