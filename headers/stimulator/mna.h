/**
 * @file mna.h
 * 
 * @brief Defines functions to perform voltage stimulation on a Nanowire
 * Network using the Modified Nodal Analysis (MNA) algorithm.
 * 
 * This file defines the functions required to perform voltage stimulation 
 * of a Nanowire Network through the Modified Nodal Analysis (MNA) algorithm. 
 * The MNA algorithm is used to calculate the voltage distribution across the 
 * network without considering its plasticity, effectively treating it as a 
 * static device. The voltage distribution is stored in the vector `V` of the 
 * `ns` parameter.
 * 
 * The file defines two different functions for the stimulation of the Nanowire
 * Network: one using an interface, and one using a MEA. The MEA implementation
 * simply converts the MEA into an interface before passing it to the main
 * function.
 * 
 * @note The MNA implementation leverages UMFPACK functions for efficient
 * computation.
 */
#ifndef MNA_H
#define MNA_H

#include "device/network.h"
#include "device/component.h"
#include "interface/interface.h"
#include "interface/mea.h"

/// @brief Perform the voltage stimulation of the Nanowire Network by
/// using the Modified Nodal Analysis algorithm. It does not update the
/// conductance value of the network, basically ignoring its plasticity
/// and treating it as a static device. The voltage distribution is
/// stored in the vector V of the \p ns parameter.
/// 
/// @note This function uses the UMFPACK functions to perform efficient
/// calculations. Previously, some algorithms for systems solution were
/// studied with the aim to find the most efficient one. The considered
/// factorization methods are the following:
/// - backward substitution -> not applicable as the elements
///   on the bottom right of the diagonal are != 0
/// - cholesky -> not applicable as the matrix is not positive-defined
/// - LU decomposition -> costs n^3
/// - Gauss-Jordan elimination -> costs n^3
/// 
/// @param[in, out] ns The Nanowire Network equivalent electrical circuit
/// on which performing the MNA. Only the voltage value of the nodes belonging
/// to the passed CC will be modified.
/// @param[in] cc The connected component of `ns` to stimulate.
/// @param[in] it The interface of the Nanowire Network with the external
/// world, including sources, grounds and loads.
/// @param[in, out] io An array with an entry for each source. As input
/// parameter it contains the voltage applied to a source, as output it
/// contains the current drawn from that node.
/// @return 0 if the computation successfully terminates, -1 if an error occurs
/// (e.g. if the sources/grounds/loads nanowires are not connected).
int voltage_stimulation(
    network_state ns,
    const connected_component cc,
    const interface it,
    double io[]
);

/// @brief Perform the voltage stimulation of a Nanowire Network connected with
/// a MEA. See ::voltage_stimulation for more details.
/// 
/// @param[in, out] ns The Nanowire Network equivalent electrical circuit
/// on which performing the MNA. Only the voltage value of the nodes belonging
/// to the passed CC will be modified.
/// @param[in] cc The connected component of `ns` to stimulate.
/// @param[in] mea The mea-interface of the Nanowire Network with the external
/// world, including sources, grounds and loads.
/// @param[in, out] io An array with an entry for each source. As input
/// parameter it contains the voltage applied to a source, as output it
/// contains the current drawn from that node.
/// @return 0 if the computation successfully terminates, -1 if an error occurs
/// (e.g. if the sources/grounds/loads nanowires are not connected).
int voltage_stimulation_mea(
    network_state ns,
    const connected_component cc,
    const MEA mea,
    double io[]
);

#endif /* MNA_H */
