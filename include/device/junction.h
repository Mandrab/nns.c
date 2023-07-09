#ifndef JUNCTION_H
#define JUNCTION_H

#include "util/point.h"

typedef struct
{
    int     first_wire;
    int     second_wire;
    point   position;
} junction;

#endif /* JUNCTION_H */
