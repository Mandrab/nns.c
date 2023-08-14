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

def draw_network(ds, nt):
    fig, ax = plt.subplots()

    ax.set(title="Network topology")
    ax.set(xlabel=r'x [$\mu$m]', ylabel=r'y [$\mu$m]')
    ax.set(xlim=(0, ds[3]), ylim=(0, ds[3]))
    ax.ticklabel_format(style='plain')
    ax.grid()

    draw_wires(ax, nt[1])
    draw_junctions(ax, nt[2])

    plt.show()

def draw_voltage_state(nt, mapping, ns):
    plt.title("Voltage distribution")

    graph = nx.from_numpy_array(ns[1])
    cc_index = max(mapping, key=mapping.count)
    position = [(w[0], w[1]) for i, w in enumerate(nt[1]) if mapping[i] == cc_index]

    nx.draw_networkx(
        graph, position,
        node_color=ns[3],
        node_size=min(5000 / graph.number_of_nodes(), 50),
        width=min(1000 / graph.number_of_edges(), 2),
        with_labels=graph.number_of_nodes() < 100
    )

    plt.colorbar(plt.cm.ScalarMappable(norm=plt.Normalize(vmin=min(ns[3]), vmax=max(ns[3]))))
    plt.show()

def draw_admittance_state(nt, mapping, ns):
    plt.title("Admittance distribution")

    graph = nx.from_numpy_array(ns[1])
    cc_index = max(mapping, key=mapping.count)
    position = [(w[0], w[1]) for i, w in enumerate(nt[1]) if mapping[i] == cc_index]
    weight = np.triu(ns[2])[np.triu(ns[2] > 0)].flatten()

    nx.draw_networkx(
        graph, position,
        node_size=min(5000 / graph.number_of_nodes(), 50),
        width=min(1000 / graph.number_of_edges(), 2),
        edge_color=weight, edge_cmap=cmap,
        with_labels=graph.number_of_nodes() < 100
    )

    plt.colorbar(plt.cm.ScalarMappable(norm=plt.Normalize(
        vmin=ns[2][ns[2] > 0].min(),
        vmax=ns[2].max()
    )))
    plt.show()

def animate_voltage_variation(nt, mapping, nss, save=False):
    fig, _ = plt.subplots()

    cc_index = max(mapping, key=mapping.count)
    position = [(w[0], w[1]) for i, w in enumerate(nt[1]) if mapping[i] == cc_index]
    vmin, vmax = min(map(lambda ns: min(ns[3]), nss)), max(map(lambda ns: max(ns[3]), nss))


    def step(ins):
        i, ns = ins

        plt.cla()
        plt.title(f"Voltage variation [step {i}]")

        graph = nx.from_numpy_array(ns[1])
        nx.draw_networkx(
            graph, position,
            node_color=ns[3],
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

    cc_index = max(mapping, key=mapping.count)
    position = [(w[0], w[1]) for i, w in enumerate(nt[1]) if mapping[i] == cc_index]
    vmin, vmax = min(map(lambda ns: ns[2][ns[2] > 0].min(), nss)), max(map(lambda ns: ns[2].max(), nss))

    def step(ins):
        i, ns = ins

        plt.cla()
        plt.title(f"Admittance variation [step {i}]")

        graph = nx.from_numpy_array(ns[1])
        weight = np.triu(ns[2])[np.triu(ns[2] > 0)].flatten()

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
