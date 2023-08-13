#!/usr/bin/python

import sys

from util.load import *
from util.plot import *

assert len(sys.argv) > 1, "The path to the file must be provided!"

network_path, interface_path, *states_path = sys.argv[1:]

ds, nt = load_network(network_path)
draw_network(ds, nt)
draw_voltage_state(load_state(states_path[-1]))
draw_admittance_state(load_state(states_path[-1]))
animate_admittance_variation(
    map(load_state, states_path)    # lazy load of the network states
)
