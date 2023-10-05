#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "device/network.h"
#include "device/component.h"

/// @brief Given a network topology and its corresponding datasheet, generate
/// an array containing for each node the index of the parent connected
/// component and return the number of connected components discovered.
/// 
/// @param[in] ds The datasheet describing the Nanowire Network.
/// @param[in] nt The topology of the Nanowire Network.
/// @param[out] n2c An array of length 'ds.wires_count' containing for each
/// entry the index of the parent connected component.
/// @return The number of connected components discovered in the NN.
int map_components(const datasheet ds, const network_topology nt, int n2c[]);

/// @brief Sort the Ws and Js arrays of `nt` according to the index of the
/// connected component each entry belongs to. The index is specified by the
/// `n2c` array. Once `nt` is sorted, also the entries of the `n2c` arrays are
/// corrected to match those of the new `nt`. E.g.:
/// ```
///     n2c = [0, 1, 0, 2, 1]
///     ws  = [a, b, c, d, e]
///              |
///             \/
///     ws  = [a, c, b, e, d]
/// ```
/// 
/// @param[in] ds The datasheet describing the Nanowire Network.
/// @param[in, out] nt The topology of the Nanowire Network to sort. Ws will be
/// sort according to the CC index, original first wire index, and original
/// second wire index. Js will be sorted according to the new indexes of the
/// wires.
/// @param[in, out] n2c An array of length 'ds.wires_count' containing for each
/// entry the index of the parent connected component. After the function call
/// it will contain the entries sorted to match the Ws wires.
/// @param[in] cc_count The number of connected components discovered in the
/// NN.
void group_nanowires(
    const datasheet ds,
    network_topology nt,
    int n2c[],
    int cc_count
);

/// @brief Create `cc_count` connected components referencing the sub-arrays of
/// `ns` containing the data belonging to them. Since the wires and the
/// junctions are ordered according to the CC index, it is enough to know the
/// starting point of the arrays (i.e., ws_skip and js_skip) and their length
/// (i.e., ws_count and js_count). Additionally, calculate and save the index
/// of the junctions in the CC-specific adjacency matrix (see Is array).
/// 
/// @param[in] ds The datasheet describing the Nanowire Network.
/// @param[in] nt The topology of the Nanowire Network.
/// @param[in] ns The dynamic electrical state of the Nanowire Network.
/// @param[in] n2c An array of length 'ds.wires_count' containing for each
/// entry the index of the parent connected component.
/// @param[in] cc_count The number of connected components discovered in the
/// NN.
/// @return The array of `cc_count` initialized connected components.
connected_component* split_components(
    const datasheet ds,
    const network_topology nt,
    const network_state ns,
    int n2c[],
    int cc_count
);

#endif /* COMPONENTS_H */
