#include <stdio.h>

#include "io/serializer.h"
#include "config.h"

#define VERSION_NUMBER "0.0.1"

int serialize_network(const datasheet ds, const network_topology nt, int id)
{
    char name[100];

    snprintf(name, 100, NETWORK_FILE_NAME_FORMAT, id);
    FILE* file = fopen(name, "w");
    if (file == NULL)
    {
        printf("Impossible to open the network file");
        return -1;
    }

    fprintf(file, "NN FILE - VERSION %s", VERSION_NUMBER);

    fprintf(file, "\n\nDATASHEET:");
    fprintf(file, "\n\twires count: %d", ds.wires_count);
    fprintf(file, "\n\twires mean length: %f", ds.length_mean);
    fprintf(file, "\n\twires standard deviation length: %f", ds.length_std_dev);
    fprintf(file, "\n\tpackage size: %d", ds.package_size);
    fprintf(file, "\n\tgeneration seed: %d", ds.generation_seed);

    fprintf(file, "\n\nTOPOLOGY:");
    for (int i = 0; i < ds.wires_count; i++)
    {
        fprintf(
            file,
            "\n\tWIRE centroid: {x: %f, y: %f}, start edge: {%f %f}, end edge: {%f %f}, length: %f",
            nt.Ws[i].centroid.x,
            nt.Ws[i].centroid.y,
            nt.Ws[i].start_edge.x,
            nt.Ws[i].start_edge.y,
            nt.Ws[i].end_edge.x,
            nt.Ws[i].end_edge.y,
            nt.Ws[i].length
        );
    }
    fprintf(file, "\n\tjunctions count: %d", nt.js_count);
    for (int i = 0; i < nt.js_count; i++)
    {
        fprintf(
            file,
            "\n\tJUNCTION i: %d, j: %d, position: {%f %f}",
            nt.Js[i].first_wire,
            nt.Js[i].second_wire,
            nt.Js[i].position.x,
            nt.Js[i].position.y
        );
    }
    fclose(file);

    return 0;
}

int serialize_state(network_state ns, int id)
{
    char name[100];

    snprintf(name, 100, STATE_FILE_NAME_FORMAT, id);
    FILE* file = fopen(name, "w");
    if (file == NULL)
    {
        printf("Impossible to open the network file");
        return -1;
    }

    fprintf(file, "NS FILE - VERSION %s", VERSION_NUMBER);

    fprintf(file, "\n\nnetwork size: %d", ns.size);

    fprintf(file, "\nA:\n\t");
    for (int i = 0; i < ns.size; i++)
    {
        for (int j = 0; j < ns.size; j++)
        {
            fprintf(file, "%d ", ns.A[i][j]);
        }
        fprintf(file, "\n\t");
    }

    fprintf(file, "\nG:\n\t");
    for (int i = 0; i < ns.size; i++)
    {
        for (int j = 0; j < ns.size; j++)
        {
            fprintf(file, "%f \t", ns.G[i][j]);
        }
        fprintf(file, "\n\t");
    }

    fprintf(file, "\nY:\n\t");
    for (int i = 0; i < ns.size; i++)
    {
        for (int j = 0; j < ns.size; j++)
        {
            fprintf(file, "%f \t", ns.Y[i][j]);
        }
        fprintf(file, "\n\t");
    }

    fprintf(file, "\nV:\n\t");
    for (int i = 0; i < ns.size; i++)
    {
        fprintf(file, "%f \t", ns.V[i]);
    }
    fprintf(file, "\n");

    fclose(file);

    return 0;
}

int serialize_interface(const interface it, int id)
{
    char name[100];

    snprintf(name, 100, INTERFACE_FILE_NAME_FORMAT, id);
    FILE* file = fopen(name, "w");
    if (file == NULL)
    {
        printf("Impossible to open the network file");
        return -1;
    }

    fprintf(file, "IT FILE - VERSION %s\n\n", VERSION_NUMBER);

    fprintf(file, "mask size: %d\n", it.mask_size);

    fprintf(file, "sources count: %d\n", it.sources_count);
    for (int i = 0; i < it.mask_size; i++)
    {
        fprintf(file, "%d ", it.sources_mask[i]);
    }

    fprintf(file, "\ngrounds count: %d\n", it.grounds_count);
    for (int i = 0; i < it.mask_size; i++)
    {
        fprintf(file, "%d ", it.grounds_mask[i]);
    }

    fprintf(file, "\nloads count: %d\n", it.loads_count);
    for (int i = 0; i < it.mask_size; i++)
    {
        fprintf(file, "%d ", it.loads_mask[i]);
    }
    fprintf(file, "\n");
    for (int i = 0; i < it.mask_size; i++)
    {
        fprintf(file, "%f ", it.loads_weight[i]);
    }

    fclose(file);

    return 0;
}
