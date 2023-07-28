#ifndef MEASURES_H
#define MEASURES_H

#include "device/network.h"

/// @brief Calculate the resistance distance between two points of
/// the Nanowire Network.
/// 
/// @param[in] ns The Nanowire Network circuital state on which
/// perform the measure.
/// @param[in] a The first point to consider for the calculation.
/// @param[in] b The second point to consider for the calculation.
/// @return The resistance of the Nanowire Network equivalent
/// circuit between the two points.
double resistive_distance(const network_state ns, int a, int b);

#endif /* MEASURES_H */
