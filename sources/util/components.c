#include "device/network.h"
#include "util/components.h"
#include "util/tensors.h"

int* map_components(bool** adj, int size);

network_state* connected_components(network_state ns, int* nss_count, int** nss_nodes)
{
    // map the node indexes with the CC indexes
    int* mapping = map_components(ns.A, ns.size);

    // set the variable to save the number of CCs (assume at least one exists)
    *nss_count = 1;

    // find the number of CCs
    for (int i = 0; i < ns.size; i++)
    {
        // if a CC has higher index, update the count
        if (mapping[i] >= *nss_count)
        {
            // the CCs index starts from 0, so the number is max + 1
            *nss_count = mapping[i] + 1;
        }
    }

    // create counters to store the number of nodes in each cc
    *nss_nodes = zeros_vector(int, *nss_count);

    // calculate the number of nodes in each CC
    for (int i = 0; i < ns.size; i++)
    {
        // increment the counter of the CC containing i
        (*nss_nodes)[mapping[i]]++;
    }

    // create a vector of network states, one for each component
    network_state* nss = vector(network_state, *nss_count);

    // create a network state for each CC
    for (int i = 0; i < *nss_count; i++)
    {
        // create a zero initialized network state
        nss[i].size = (*nss_nodes)[i];
        nss[i].A = zeros_matrix(bool, (*nss_nodes)[i], (*nss_nodes)[i]);
        nss[i].G = zeros_matrix(float, (*nss_nodes)[i], (*nss_nodes)[i]);
        nss[i].Y = zeros_matrix(float, (*nss_nodes)[i], (*nss_nodes)[i]);
        nss[i].V = zeros_vector(float, (*nss_nodes)[i]);
    }

    // create counters to memorize how many nodes belonging to each CC were visited
    int x_indexes[*nss_count] = { };
    int y_indexes[*nss_count] = { };

    // iterate each node of the network to set it into the corresponding network state
    for (int i = 0; i < ns.size; i++)
    {
        // retrieve the index of the CC containing i
        int ci = mapping[i];

        // get the number of elements memorized in the x axis and increment the counter
        int cix = x_indexes[ci]++;

        // reset the counter of the y axis
        y_indexes[ci] = 0;

        // iterate all possible neighbors
        for (int j = 0; j < ns.size; j++)
        {
            // if the two nodes do not belong to the same component, continue
            if (ci != mapping[j])
            {
                continue;
            }

            // get the number of elements memorized in the y axis and increment the counter
            int ciy = y_indexes[ci]++;

            // set the matrixes in the network state with the correct informations
            nss[ci].A[cix][ciy] = ns.A[i][j];
            nss[ci].G[cix][ciy] = ns.G[i][j];
            nss[ci].Y[cix][ciy] = ns.Y[i][j];
        }

        // set the voltages vector with the correct informations
        nss[ci].V[cix] = ns.V[i];
    }

    return nss;
}

network_state* largest_component(network_state* nss, int nss_count, int* nodes_count)
{
    int largest_cc_index = 0;

    // find the index of the largest connected component
    for (int i = 0, max = 0; i < nss_count; i++)
    {
        if (nodes_count[i] > max)
        {
            largest_cc_index = i;
            max = nodes_count[i];
        }
    }

    return &nss[largest_cc_index];
}

int* map_components(bool** adj, int size)
{
    // create a mapping from node to connected component
    int* mapping = zeros_vector(int, size);

    // create vectors to mark the nodes as visited
    // or discovered as adjacent but not yet visited
    bool visited[size] = { };
    bool discovered[size] = { };

    // iterate all the nodes to discover their adjacency
    for (int i = 0, cc_count = 0; i < size; i++)
    {
        // if the node is already visited, continue to the next one
        if (visited[i])
        {
            continue;
        }

        // create a stack to store the non-visited neighbors and add the origin
        int stack[size];
        int top = 0;
        stack[top] = i;

        // iterate while the stack is not empty
        while (top >= 0)
        {
            // remove an element from the stack and mark it as visited
            int node = stack[top--];
            visited[node] = true;
            mapping[node] = cc_count;

            // check all the nodes to find the nodes neighbors
            for (int j = 0; j < size; j++)
            {
                // if the node is adjacent and not yet discovered add it to the stack
                if (adj[node][j] && ! discovered[j])
                {
                    stack[++top] = j;
                    discovered[j] = true;
                }
            }
        }

        // increment the counter of the connected components
        cc_count++;
    }

    return mapping;
}
