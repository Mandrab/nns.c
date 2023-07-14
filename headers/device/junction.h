#ifndef JUNCTION_H
#define JUNCTION_H

#include "util/point.h"

/// Stores the index of the wires forming the junction together with its position.
typedef struct
{
    int     first_wire;     ///< First wire forming the junction.
    int     second_wire;    ///< Second wire forming the junction.
    point   position;       ///< Position of the junction.
} junction;

#endif /* JUNCTION_H */
