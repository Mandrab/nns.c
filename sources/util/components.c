#include <string.h>

#include "device/network.h"
#include "util/components.h"
#include "util/tensors.h"

int map_components(const datasheet ds, const network_topology nt, int* mapping)
{
    // create a union-find data structure to discover the connected components
    // rank contains the depth of a tree, sets contains the parent of a node
    int rank[ds.wires_count] = { };
    int sets[ds.wires_count];

    // initialize the sets array by creating a connected component
    // for each nanowire; e.g.: nanowire_0 \in CC_0 .. nanowire_n \in CC_n
    for (int i = 0; i < ds.wires_count; i++)
    {
        sets[i] = i;
    }

    // iterate the junctions and merge the connected components
    for (int k = 0; k < nt.js_count; k++)
    {
        // get a pair of connected nanowires
        int i = nt.Js[k].first_wire;
        int j = nt.Js[k].second_wire;

        // find the root of the parent connected component of i
        while (i != sets[i])
        {
            i = sets[i];
        }

        // find the root of the parent connected component of j
        while (j != sets[j])
        {
            j = sets[j];
        }

        // if the two connected components are already joined, continue
        if (i == j)
        {
            continue;
        }

        // joint the two connected components according to their rank
        // i.e., to the depth of their tree
        if (rank[i] < rank[j])
        {
            sets[i] = j;
        }
        else
        if (rank[i] > rank[j])
        {
            sets[j] = i;
        }
        // if the two trees are of the same length, increase it and join them
        else
        {
            sets[j] = i;
            rank[i]++;
        }
    }

    // substitute the parent of a node with the root of the connected component
    for (int i = 0; i < ds.wires_count; i++)
    {
        mapping[i] = i;
        while (mapping[i] != sets[mapping[i]])
        {
            mapping[i] = sets[mapping[i]];
        }
    }

    // count the unique connected components by counting their roots and
    // memorize their index to perform a renaming in range [0, cc_count]
    int remap[ds.wires_count];
    int cc_count = 0;
    for (int i = 0; i < ds.wires_count; i++)
    {
        if (i == sets[i])
        {
            remap[i] = cc_count++;
        }
    }

    // rename the connected components starting from 0
    for (int i = 0; i < ds.wires_count; i++)
    {
        mapping[i] = remap[mapping[i]];
    }

    return cc_count;
}

void group_nanowires(const datasheet ds, network_topology nt, int* n2c, int cc_count)
{
    // count the number of nanowires in each connected component
    int counter[cc_count] = { };
    for (int i = 0; i < ds.wires_count; i++)
    {
        counter[n2c[i]]++;
    }

    // calculate the number of nanowires preceding each CC
    int start_index[cc_count] = { };
    for (int i = 1; i < cc_count; i++)
    {
        start_index[i] = start_index[i - 1] + counter[i - 1];
    }

    // create an array to contain the sorted wires of the network topology
    wire Ws[ds.wires_count];
    memcpy(Ws, nt.Ws, ds.wires_count * sizeof(wire));

    // create an array to contain the old node to CC mapping
    int old_n2c[ds.wires_count];
    memcpy(old_n2c, n2c, ds.wires_count * sizeof(int));

    // re-map the nanowires index by grouping them according to their CC
    // and set copy the wire information from the old to the new array
    int mapping[ds.wires_count];
    for (int i = 0; i < ds.wires_count; i++)
    {
        mapping[i] = start_index[old_n2c[i]]++;
        nt.Ws[mapping[i]] = Ws[i];
        n2c[mapping[i]] = old_n2c[i];
    }

    // rename the junctions wires and sort them
    for (int i = 0; i < nt.js_count; i++)
    {
        nt.Js[i].first_wire = mapping[nt.Js[i].first_wire];
        nt.Js[i].second_wire = mapping[nt.Js[i].second_wire];
    }
    qsort(nt.Js, nt.js_count, sizeof(junction), jcmp);
}

connected_component* split_components(
    const datasheet ds,
    const network_topology nt,
    const network_state ns,
    int* n2c,
    int cc_count
)
{
    // create an empty array of connected components
    connected_component* ccs = zeros_vector(connected_component, cc_count);

    // increment the number of nodes in the CC containing the nanowire
    for (int i = 0; i < ds.wires_count; i++)
    {
        ccs[n2c[i]].ws_count++;
    }

    // increment the number of edges in the CC containing the junction
    for (int i = 0; i < nt.js_count; i++)
    {
        ccs[n2c[nt.Js[i].first_wire]].js_count++;
    }

    // initialize Is only if the CC contains edges (i.e., |nodes| > 1)
    for (int i = 0; i < cc_count; i++)
    {
        if (ccs[i].js_count > 0)
        {
            ccs[i].Is = vector(int, ccs[i].js_count);
        }
    }

    // calculate the number of nanowires preceding each CC
    for (int i = 1; i < cc_count; i++)
    {
        ccs[i].ws_skip = ccs[i - 1].ws_skip + ccs[i - 1].ws_count;
        ccs[i].js_skip = ccs[i - 1].js_skip + ccs[i - 1].js_count;
    }

    int counters[cc_count] = { };
    for (int k = 0; k < nt.js_count; k++)
    {
        int cci = n2c[nt.Js[k].first_wire];
        int i = nt.Js[k].first_wire - ccs[cci].ws_skip;
        int j = nt.Js[k].second_wire - ccs[cci].ws_skip;

        // calculate and set the junction position in the linearized
        // adjacency matrix of the connected component
        ccs[cci].Is[counters[cci]++] = i * ccs[cci].ws_count + j;
    }

    return ccs;
}
