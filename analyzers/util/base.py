from matplotlib.lines import Line2D

def draw_wires(ax, Ws):
    """
    Draw the nano-wires composing the network.
    The centre of the wires is marked with a red marker. 

    ax -- matplotlib axes to draw on.
    Ws -- the set of wires to draw.
    """

    for xc, yc, xa, ya, xb, yb, _ in Ws:
        ax.add_line(Line2D([xa, xb], [ya, yb], color=(0.42, 0.42, 0.42)))
        ax.add_line(Line2D([xc], [yc], color='r', marker='o', ms=2))

def draw_junctions(ax, Js):
    """
    Draw the junctions as circles.

    ax -- matplotlib axes to draw on.
    Js -- the set of junctions to draw.
    """

    for _, _, x, y in Js:
        ax.add_line(Line2D([x], [y], color='b', marker='o', ms=2))
