#ifndef JUNCTION_H
#define JUNCTION_H

#include "util/point.h"

/// @brief Stores the index of the wires forming the junction together with its
/// position.
typedef struct
{
    int     first_wire;     ///< First wire forming the junction.
    int     second_wire;    ///< Second wire forming the junction.
    point   position;       ///< Position of the junction.
} junction;

/// @brief Compares two junctions according to their first and potentially
/// second nanowire-index. Intended to be used with the qsort function.
/// 
/// @param[in] e1 Pointer to the first element to compare.
/// @param[in] e2 Pointer to the second element to compare.
/// @return 0 if the junctions have the same first and second index; < 0 if e1
/// has lower first index (or in case of equality lower second index) than e2;
/// > 0 otherwise.
int jcmp(const void* e1, const void* e2);

#endif /* JUNCTION_H */
