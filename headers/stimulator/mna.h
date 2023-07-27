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
/// @param[in] inputs The voltage potential on the source nodes ordered
/// according to their index.
void voltage_stimulation(network_state nw, const interface it, const double* inputs);

#endif /* MNA_H */
