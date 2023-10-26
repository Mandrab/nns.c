#ifndef MEA_H
#define MEA_H

#include "config.h"
#include "device/datasheet.h"
#include "device/network.h"
#include "util/point.h"

/// @brief Represent a Micro-Electrode Array of 16 electrodes (physically
/// sound). Save their position and the index of the nearest nanowire
/// (calculated from the centroid). If the electrode is not enough near to any
/// nanowire, its mapping value is -1.
typedef struct
{
    point Ps[MEA_ELECTRODES];   ///< The position of each electrode.
    int e2n[MEA_ELECTRODES];    ///< A mapping from the electrode index to the
                                ///< nanowire index. If the electrode is not
                                ///< enough near to any nanowire, its value is
                                ///< -1.
} MEA;

/// @brief Connect the MEA to the Nanowire Network discovering the touched
/// nanowires.
/// 
/// @param [in] ds The datasheet describing the Nanowire Network.
/// @param [in] nt The topology of the Nanowire Network.
/// @return The MEA data-structure containing the electrodes position and the
/// mapping from them to the touched nanowire.
MEA connect_MEA(const datasheet ds, const network_topology nt);

#endif /* MEA_H */
