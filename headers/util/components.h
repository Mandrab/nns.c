#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "device/network.h"
#include "device/component.h"

/// @brief Given a network topology and its corresponding datasheet, returns an
/// array containing for each node the index of the parent connected component.
/// 
/// @param[in] ds The datasheet describing the Nanowire Network.
/// @param[in] nt The topology of the Nanowire Network.
/// @param[out] cc_count An integer to be set equal to the number of connected
/// components in the NN.
/// @return An array of length 'ds.wires_count' containing for each entry the
/// index of the parent connected component.
int map_components(const datasheet ds, const network_topology nt, int* mapping);

void group_nanowires(const datasheet ds, network_topology nt, int* mapping, int cc_count);

connected_component* split_components(
    const datasheet ds,
    const network_topology nt,
    const network_state ns,
    int* n2c,
    int cc_count
);


// /// @brief Separate the Nanowire Network state in connected components (i.e.,
// /// connected groups of nanowires) to be used for the voltage stimulation. The
// /// Ys array in each CC references a sub-section of the array in `ns'. For this
// /// motivation Ys in the `ns' should be considered as ordered according to the
// /// connected component. As the function does not explicitly order the array, a
// /// non-stimulated Nanowire Network is expected as input, i.e., with all its
// /// elements set to Y_MIN.
// /// E.g. (the values in the `nn state' are purely imaginary):
// ///     nn state = [
// ///         0, 0, 0, 1, 0
// ///         0, 2, 0, 0, 0
// ///         0, 0, 3, 0, 0
// ///         4, 0, 0, 0, 0
// ///         0, 0, 0, 0, 5
// ///     ]
// ///     CC mapping =    [0,  0,  1,  0,  2]
// ///
// ///     initial ns Is = [3,  6,  12,  15, 24]
// ///     final ns Is =   [3,  6,  15,  12, 24]
// /// 
// ///     initial ns Ys = [1,  2,  3,  4,  5]
// ///     final ns Ys =   [1,  2,  4,  3,  5]
// /// 
// ///     CC 0 Is =       [2,  4, 6]
// ///     CC 1 Is =       [0]
// ///     CC 2 Is =       [0]
// /// 
// ///     CC 0 Ys =       [1,  2,  4]
// ///     CC 1 Ys =       [3]
// ///     CC 2 Ys =       [5]
// /// 
// /// @param[in] ds The datasheet describing the Nanowire Network.
// /// @param[in] nt The topology of the Nanowire Network.
// /// @param[in, out] ns The potentially disconnected Nanowire Network state. The
// /// Is indexes will be ordered according to the connected component index.
// /// @param[out] nss_count An output integer to be set to the number of
// /// connected components discovered.
// /// @return The array of discovered connected components referencing the state
// /// of the Nanowire Network.
// connected_component* connected_components(
//     const datasheet ds,
//     const network_topology nt,
//     network_state ns,
//     int* nss_count
// );

#endif /* COMPONENTS_H */
