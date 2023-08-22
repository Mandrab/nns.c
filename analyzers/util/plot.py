import matplotlib.colors as clrs
import matplotlib.patches as patches
import matplotlib.pyplot as plt
import networkx as nx
import numpy as np

from matplotlib.animation import FuncAnimation, PillowWriter
from .base import draw_wires, draw_junctions

# set the default colormap and figsize
plt.rcParams["image.cmap"] = cmap = clrs.LinearSegmentedColormap.from_list("", ["gold", "red", "brown", "black"])
plt.rcParams['figure.figsize'] = (10, 10)

def draw_topology(ds, nt):
    fig, ax = plt.subplots()

    ax.set(title="Network topology")
    ax.set(xlabel=r'x [$\mu$m]', ylabel=r'y [$\mu$m]')
    ax.set(xlim=(0, ds.package_size), ylim=(0, ds.package_size))
    ax.ticklabel_format(style='plain')
    ax.grid()

    draw_wires(ax, nt.Ws, ds.wires_count)
    draw_junctions(ax, nt.Js, ds.wires_count)

    plt.show()

def draw_network_(nt, mapping, ns, **options):
    graph = nx.from_numpy_array(ns.np_A())
    cc_index = max(mapping, key=mapping.tolist().count)
    position = [
        (nt.Ws[i].centroid.x, nt.Ws[i].centroid.y)
        for i, m in enumerate(mapping) if m == cc_index
    ]

    nx.draw_networkx(
        graph, position,
        node_size=min(5000 / graph.number_of_nodes(), 50),
        width=min(1000 / graph.number_of_edges(), 2),
        with_labels=graph.number_of_nodes() < 100,
        **options
    )

def draw_voltage_state(nt, mapping, ns):
    plt.title("Voltage distribution")

    draw_network_(nt, mapping, ns, node_color=ns.np_V())

    plt.colorbar(plt.cm.ScalarMappable(norm=plt.Normalize(vmin=min(ns.np_V()), vmax=max(ns.np_V()))))
    plt.show()

def draw_admittance_state(nt, mapping, ns):
    plt.title("Admittance distribution")

    weight = np.triu(ns.np_Y()).flatten()
    weight = weight[weight > 0]
    draw_network_(nt, mapping, ns, edge_color=weight, edge_cmap=cmap)

    plt.colorbar(plt.cm.ScalarMappable(norm=plt.Normalize(
        vmin=ns.np_Y()[ns.np_Y() > 0].min(),
        vmax=ns.np_Y().max()
    )))
    plt.show()

def animate_voltage_variation(nt, mapping, nss, save=False):
    fig, _ = plt.subplots()

    cc_index = max(mapping, key=mapping.tolist().count)
    position = [
        (nt.Ws[i].centroid.x, nt.Ws[i].centroid.y)
        for i, m in enumerate(mapping) if m == cc_index
    ]
    vmin, vmax = min(map(lambda ns: min(ns.np_V()), nss)), max(map(lambda ns: max(ns.np_V()), nss))

    def step(ins):
        i, ns = ins

        plt.cla()
        plt.title(f"Voltage variation [step {i}]")

        graph = nx.from_numpy_array(ns.np_A())
        nx.draw_networkx(
            graph, position,
            node_color=ns.np_V(),
            node_size=min(5000 / graph.number_of_nodes(), 50),
            width=min(1000 / graph.number_of_edges(), 2),
            with_labels=graph.number_of_nodes() < 100,
            vmin=0, vmax=5
        )

    ani = FuncAnimation(fig, step, interval=100, blit=False, repeat=True, frames=enumerate(nss))
    if save:
        ani.save("conductance_variation.gif", dpi=300, writer=PillowWriter(fps=25))

    plt.colorbar(plt.cm.ScalarMappable(norm=plt.Normalize(vmin=0, vmax=5)))
    plt.show()

def animate_admittance_variation(nt, mapping, nss, save=False):
    fig, _ = plt.subplots()

    cc_index = max(mapping, key=mapping.tolist().count)
    position = [
        (nt.Ws[i].centroid.x, nt.Ws[i].centroid.y)
        for i, m in enumerate(mapping) if m == cc_index
    ]
    vmin, vmax = min(map(lambda ns: ns.np_Y()[ns.np_Y() > 0].min(), nss)), max(map(lambda ns: ns.np_Y().max(), nss))

    def step(ins):
        i, ns = ins

        plt.cla()
        plt.title(f"Admittance variation [step {i}]")

        graph = nx.from_numpy_array(ns.np_A())
        weight = np.triu(ns.np_Y()).flatten()
        weight = weight[weight > 0]

        nx.draw_networkx(
            graph, position,
            node_size=min(5000 / graph.number_of_nodes(), 50),
            width=min(1000 / graph.number_of_edges(), 2),
            edge_color=weight, edge_cmap=cmap,
            with_labels=graph.number_of_nodes() < 100,
            edge_vmin=vmin, edge_vmax=vmax
        )

    ani = FuncAnimation(fig, step, interval=100, blit=False, repeat=True, frames=enumerate(nss))
    if save:
        ani.save("conductance_variation.gif", dpi=300, writer=PillowWriter(fps=25))

    plt.colorbar(plt.cm.ScalarMappable(norm=plt.Normalize(vmin=vmin, vmax=vmax)))
    plt.show()
