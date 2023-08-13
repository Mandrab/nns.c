import matplotlib.patches as patches
import matplotlib.pyplot as plt
import networkx as nx
import numpy as np

from matplotlib.animation import FuncAnimation, PillowWriter
from .base import draw_wires, draw_junctions

# set the default colormap and figsize
plt.rcParams["image.cmap"] = "coolwarm"
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

def draw_voltage_state(ns):
    plt.title("Voltage distribution")

    graph = nx.from_numpy_array(np.matrix(ns[1]))
    nx.draw_kamada_kawai(
        graph,
        node_size=min(5000 / graph.number_of_nodes(), 50),
        width=min(1000 / graph.number_of_edges(), 2),
        node_color=ns[3]
    )

    plt.colorbar(plt.cm.ScalarMappable(norm=plt.Normalize(vmin=min(ns[3]), vmax=max(ns[3]))))
    plt.show()

def draw_conductance_state(ns):
    plt.title("Conductance distribution")

    graph = nx.from_numpy_array(np.matrix(ns[1]))
    weight = [ns[2][i][j] for i, j in graph.edges()]

    nx.draw_kamada_kawai(
        graph,
        node_size=min(5000 / graph.number_of_nodes(), 50),
        width=min(1000 / graph.number_of_edges(), 2),
        edge_color=weight, edge_cmap=plt.cm.coolwarm
    )

    plt.colorbar(plt.cm.ScalarMappable(norm=plt.Normalize(vmin=min(ns[3]), vmax=max(ns[3]))))
    plt.show()

def animate_conductance_variation(nss, save=False):
    fig, _ = plt.subplots()

    def step(ins):
        i, ns = ins

        plt.cla()
        plt.title(f"Conductance variation [step {i}]")

        graph = nx.from_numpy_array(np.matrix(ns[1]))
        weight = [ns[2][i][j] for i, j in graph.edges()]

        nx.draw_kamada_kawai(
            graph,
            node_size=min(5000 / graph.number_of_nodes(), 50),
            width=min(1000 / graph.number_of_edges(), 2),
            edge_color=weight, edge_cmap=plt.cm.coolwarm
        )

    ani = FuncAnimation(fig, step, interval=250, blit=False, repeat=True, frames=enumerate(nss))
    if save:
        ani.save("conductance_variation.gif", dpi=300, writer=PillowWriter(fps=25))

    plt.colorbar(plt.cm.ScalarMappable(norm=plt.Normalize(vmin=0.001, vmax=0.1)))
    plt.show()
