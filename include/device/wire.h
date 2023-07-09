#ifndef WIRE_H
#define WIRE_H

#include "util/point.h"

typedef struct
{
    point centroid;
    point start_edge;
    point end_edge;
    float length;
} wire;

#endif /* WIRE_H */
