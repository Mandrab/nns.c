import numpy as np
import struct

VERSION_NUMBER = 0
BOOL_SIZE = 1
INT_SIZE = 4
DOUBLE_SIZE = 8

def load_network(path):
    """Load the network topology saved in "*.nn.nns" files."""

    with open(path, 'rb') as file:

        version = struct.unpack("<i", file.read(INT_SIZE))[0]
        assert version == VERSION_NUMBER, "The file version is not supported by this version of the simulator"

        # DATASHEET READING

        datasheet = struct.unpack("<iddii", file.read(3 * INT_SIZE + 2 * DOUBLE_SIZE))

        # print(datasheet)

        # TOPOLOGY READING

        js_count = struct.unpack("<i", file.read(INT_SIZE))[0]

        Ws = struct.unpack(f"<{datasheet[0] * 7}d", file.read(datasheet[0] * 7 * DOUBLE_SIZE))
        Ws = iter(Ws)
        Ws = list(zip(
            Ws, Ws, # centroid
            Ws, Ws, # start
            Ws, Ws, # end
            Ws      # length
        ))

        Js = [(
            *struct.unpack("<2i", file.read(2 * INT_SIZE)),     # first and second wire
            *struct.unpack("<2d", file.read(2 * DOUBLE_SIZE)),  # position
            ) for _ in range(js_count)
        ]

        # CC MAP READING

        mapping = struct.unpack(f"<{datasheet[0]}i", file.read(datasheet[0] * INT_SIZE))

        # print(js_count, Ws[:5], Js[:5])

        return datasheet, (js_count, Ws, Js), mapping

def load_state(path):
    """Load the network state saved in "*.ns.nns" files."""

    with open(path, 'rb') as file:

        version = struct.unpack("<i", file.read(INT_SIZE))[0]
        assert version == VERSION_NUMBER, "The file version is not supported by this version of the simulator"

        size = struct.unpack("<i", file.read(INT_SIZE))[0]

        A = np.matrix([
            struct.unpack(f"<{size}?", file.read(size * BOOL_SIZE))
            for _ in range(size)
        ])

        Y = np.matrix([
            struct.unpack(f"<{size}d", file.read(size * DOUBLE_SIZE))
            for _ in range(size)
        ])

        V = struct.unpack(f"<{size}d", file.read(size * DOUBLE_SIZE))

        return size, A, Y, V
