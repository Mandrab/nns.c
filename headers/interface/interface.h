#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdbool.h>

/// @brief Interface between the nanowire-network device and external systems.
/// 
/// @details Example of the possible use of the interface.
/// ```
///             V
///             |
///          ---------------
///         |   |           |
///         |   *---*---*   |
///         |   |  /    |   |
///         |   | /     |   |
///         |   *===L===*   |
///         | NN        |   |
///          ---------------
///                     |
///                     G
/// ```
typedef struct
{
    int     mask_size;      ///< The size of the nodes masks.

    int     sources_count;  ///< Number of sources connected to the device.
    bool*   sources_mask;   ///< Mask of the source nodes (TRUE = source).

    int     grounds_count;  ///< Number of grounds connected to the device.
    bool*   grounds_mask;   ///< Mask of the grounded nodes (TRUE = grounded).

    int     loads_count;    ///< Number of loads connected to the device.
    bool*   loads_mask;     ///< Mask of the loaded nodes (TRUE = loaded).
    double* loads_weight;   ///< Weight of the optional load connected to the node (in siemens).
} interface;

#endif /* INTERFACE_H */
