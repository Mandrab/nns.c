import numpy as np

from ctypes import *

class datasheet(Structure):
    _fields_ = [
        ("wires_count", c_int),
        ("length_mean", c_double),
        ("length_std_dev", c_double),
        ("package_size", c_int),
        ("generation_seed", c_int)
    ]

class position(Structure):
    _fields_ = [
        ("x", c_double),
        ("y", c_double)
    ]

class wire(Structure):
    _fields_ = [
        ("centroid", position),
        ("start_edge", position),
        ("end_edge", position),
        ("length", c_double)
    ]

class junction(Structure):
    _fields_ = [
        ("first_wire", c_int),
        ("second_wire", c_int),
        ("position", position)
    ]

class network_topology(Structure):
    _fields_ = [
        ("Ws", POINTER(wire)),
        ("js_count", c_int),
        ("Js", POINTER(junction))
    ]

class network_state(Structure):
    _fields_ = [
        ("size", c_int),
        ("A", POINTER(POINTER(c_bool))),
        ("Y", POINTER(POINTER(c_double))),
        ("V", POINTER(c_double))
    ]
    def np_A(self): return p2m(c_bool, np.bool_, self.A, self.size, self.size)
    def np_Y(self): return p2m(c_double, np.float64, self.Y, self.size, self.size)
    def np_V(self): return p2a(c_double, np.float64, self.V, self.size)

def p2a(ctype, nptype, pointer, length):
    """Pointer to np.array."""
    pointer = cast(pointer, POINTER(ctype * length))[0]
    return np.frombuffer(pointer, nptype)

def p2m(ctype, nptype, pointer, width, height):
    """Pointer to np.matrix."""
    return np.array([[pointer[i][j] for j in range(height)] for i in range(width)])
