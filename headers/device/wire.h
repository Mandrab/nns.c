#ifndef WIRE_H
#define WIRE_H

#include "util/point.h"

/// Stores the starting and ending positions of a wire together with its centroid position and total length.
typedef struct
{
    point centroid;     ///< Position of the centroid of the wire.
    point start_edge;   ///< Position of one of the edges of the wire.
    point end_edge;     ///< Position of one of the edges of the wire.
    float length;       ///< Total length of the wire.
} wire;

#endif /* WIRE_H */
