from matplotlib.lines import Line2D

def draw_wires(ax, Ws, count):
    """
    Draw the nano-wires composing the network.
    The centre of the wires is marked with a red marker. 

    ax -- matplotlib axes to draw on.
    Ws -- the set of wires to draw.
    """

    for w in Ws[:count]:
        ax.add_line(Line2D(
            [w.start_edge.x, w.end_edge.x],
            [w.start_edge.y, w.end_edge.y],
            color=(0.42, 0.42, 0.42)
        ))
        ax.add_line(Line2D(
            [w.centroid.x], [w.centroid.y],
            color='r', marker='o', ms=2
        ))

def draw_junctions(ax, Js, count):
    """
    Draw the junctions as circles.

    ax -- matplotlib axes to draw on.
    Js -- the set of junctions to draw.
    """

    for j in Js[:count]:
        ax.add_line(Line2D(
            [j.position.x], [j.position.y],
            color='b', marker='o', ms=2
        ))
