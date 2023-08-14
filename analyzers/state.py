#!/usr/bin/python

import sys

from util.load import *
from util.plot import *

assert len(sys.argv) > 1, "The path to the file must be provided!"

network_path, interface_path, *states_path = sys.argv[1:]

ds, nt, mapping = load_network(network_path)
nss = list(map(load_state, states_path))

draw_network(ds, nt)
draw_voltage_state(nt, mapping, load_state(states_path[-1]))
draw_admittance_state(nt, mapping, load_state(states_path[-1]))
animate_voltage_variation(nt, mapping, nss)
animate_admittance_variation(nt, mapping, nss)
