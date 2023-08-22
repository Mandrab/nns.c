#!/usr/bin/python

import numpy as np
import sys

from util.plot import *
from util.types import *

assert len(sys.argv) > 1, "The ID and number of snapshots to process must be passed as input!"

nns = CDLL("./libnns.so")

# get the device id and the count of networks from the command line
device_id, state_count = sys.argv[1:]
device_id, state_count = int(device_id), int(state_count)

# create the data structure to contain the deserialized data
ds = datasheet()
nt = network_topology()
full_ns = network_state()

# deserialize the static components of the network
nns.deserialize_network(pointer(ds), pointer(nt), device_id)

# deserialize the original state of the nn (including the unconnected components)
nns.deserialize_state(pointer(full_ns), device_id, -1)

# obtain the mapping of the nodes to the parent connected component
nns.map_components.restype = POINTER(c_int * full_ns.size)
mapping = nns.map_components(full_ns.A, full_ns.size)
mapping = p2a(c_int, np.int32, mapping, full_ns.size)

nss = [network_state() for _ in range(state_count)]
for i, ns in enumerate(nss):
    nns.deserialize_state(pointer(ns), device_id, i)

draw_topology(ds, nt)
draw_voltage_state(nt, mapping, nss[-1])
draw_admittance_state(nt, mapping, nss[-1])
animate_voltage_variation(nt, mapping, nss)
animate_admittance_variation(nt, mapping, nss)
