#ifndef WIRES_H
#define WIRES_H

#include <stdbool.h>

#include "device/datasheet.h"
#include "device/junction.h"
#include "device/network.h"
#include "device/wire.h"

wire* drop_wires(datasheet ds);

void detect_junctions(datasheet ds, wire* ws, junction** js, int* js_count);

bool** construe_adjacency_matrix(datasheet ds, network_topology nt);

#endif /* WIRES_H */
