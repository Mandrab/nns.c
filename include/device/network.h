#ifndef NETWORK_H
#define NETWORK_H

#include <stdbool.h>

#include "datasheet.h"
#include "junction.h"
#include "wire.h"

typedef struct
{
    wire*       Ws;

    int         js_count;
    junction*   Js;
} network_topology;

typedef struct
{
    int     size;
    bool**  A;
    float** G;
    float** Y;
    float*  V;
} network_state;

network_topology create_network(datasheet ds);

network_state construe_circuit(datasheet ds, network_topology nt);

#endif /* NETWORK_H */
