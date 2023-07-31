#ifndef MNA_H
#define MNA_h

#include "device/network.h"
#include "interface/interface.h"

/// @brief Perform the voltage stimulation of the Nanowire Network by
/// using the Modified Nodal Analysis algorithm. It does not update the
/// conductance value of the network, basically ignoring its plasticity
/// and treating it as a static device. The voltage distribution is
/// stored in the vector V of the \p ns parameter.
/// 
/// @note This function uses the LAPACK functions to perform efficient
/// calculations. Previously, some algorithms for systems solution were
/// studied with the aim to find the most efficient one. The considered
/// factorization methods are the following:
/// - backward substitution -> not applicable as the elements
///   on the bottom right of the diagonal are != 0
/// - cholesky -> not applicable as the matrix is
///   not positive-defined
/// - LU decomposition -> costs n^3
/// - Gauss-Jordan elimination -> costs n^3
/// 
/// @param[in, out] nw The Nanowire Network equivalent electrical circuit
/// on which performing the MNA.
/// @param[in] it The interface of the Nanowire Network with the external
/// world, including sources, grounds and loads.
/// @param[in, out] io An array with an entry for each source. As input
/// parameter it contains the voltage applied to the source, as output it
/// contains the current drawn from that node. The values in the array are
/// ordered according to the index of the corresponding source. E.g.:
///     node: 5, volt: 5, current: 10
///     node: 7, volt: 3, current: 8
///     io as input: [5, 3]
///     io as output: [10, 8]
void voltage_stimulation(network_state nw, const interface it, double* io);

#endif /* MNA_H */
