#!/usr/bin/python

import sys

from util.load import load_network, load_state
from util.plot import draw_network, draw_voltage_state, draw_conductance_state

assert len(sys.argv) > 1, "The path to the file must be provided!"

network_path, state_path = sys.argv[1:]

ds, nt = load_network(network_path)
ns = load_state(state_path)
draw_network(ds, nt)
draw_voltage_state(ns)
draw_conductance_state(ns)
