#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io/deserializer.h"
#include "util/tensors.h"
#include "config.h"

#define VERSION_NUMBER "0.0.1"

int deserialize_network(datasheet* ds, network_topology* nt, int id)
{
    char name[100], version[100];

    snprintf(name, 100, NETWORK_FILE_NAME_FORMAT, id);
    FILE* file = fopen(name, "r");
    if (file == NULL)
    {
        printf("Impossible to open the network file");
        return -1;
    }

    fscanf(file, "NN FILE - VERSION %s", version);
    if (strcmp(version, VERSION_NUMBER) != 0)
    {
        printf("The file version is not supported by this version of the simulator");
        return -1;
    }

    fscanf(file, "\n\nDATASHEET:");

    fscanf(file, "\n\twires count: %d", &ds->wires_count);
    fscanf(file, "\n\twires mean length: %lf", &ds->length_mean);
    fscanf(file, "\n\twires standard deviation length: %lf", &ds->length_std_dev);
    fscanf(file, "\n\tpackage size: %d", &ds->package_size);
    fscanf(file, "\n\tgeneration seed: %d", &ds->generation_seed);

    fscanf(file, "\n\nTOPOLOGY:");

    nt->Ws = vector(wire, ds->wires_count);

    for (int i = 0; i < ds->wires_count; i++)
    {
        fscanf(
            file,
            "\n\tWIRE centroid: {x: %lf, y: %lf}, start edge: {%lf %lf}, end edge: {%lf %lf}, length: %lf",
            &nt->Ws[i].centroid.x,
            &nt->Ws[i].centroid.y,
            &nt->Ws[i].start_edge.x,
            &nt->Ws[i].start_edge.y,
            &nt->Ws[i].end_edge.x,
            &nt->Ws[i].end_edge.y,
            &nt->Ws[i].length
        );
    }
    fscanf(file, "\n\tjunctions count: %d", &nt->js_count);

    nt->Js = vector(junction, nt->js_count);

    for (int i = 0; i < nt->js_count; i++)
    {
        fscanf(
            file,
            "\n\tJUNCTION i: %d, j: %d, position: {%lf %lf}",
            &nt->Js[i].first_wire,
            &nt->Js[i].second_wire,
            &nt->Js[i].position.x,
            &nt->Js[i].position.y
        );
    }
    fclose(file);

    return 0;
}

int deserialize_state(network_state* ns, int id)
{
    char name[100], version[100];

    snprintf(name, 100, STATE_FILE_NAME_FORMAT, id);
    FILE* file = fopen(name, "r");
    if (file == NULL)
    {
        printf("Impossible to open the network file");
        return -1;
    }

    fscanf(file, "NS FILE - VERSION %s", version);
    if (strcmp(version, VERSION_NUMBER) != 0)
    {
        printf("The file version is not supported by this version of the simulator");
        return -1;
    }

    fscanf(file, "\n\nnetwork size: %d", &ns->size);

    ns->A = matrix(bool, ns->size, ns->size);
    ns->G = matrix(double, ns->size, ns->size);
    ns->Y = matrix(double, ns->size, ns->size);
    ns->V = vector(double, ns->size);

    fscanf(file, "\nA:\n\t");
    for (int i = 0; i < ns->size; i++)
    {
        for (int j = 0; j < ns->size; j++)
        {
            fscanf(file, "%d ", &ns->A[i][j]);
        }
        fscanf(file, "\n\t");
    }

    fscanf(file, "\nG:\n\t");
    for (int i = 0; i < ns->size; i++)
    {
        for (int j = 0; j < ns->size; j++)
        {
            fscanf(file, "%lf ", &ns->G[i][j]);
        }
        fscanf(file, "\n\t");
    }

    fscanf(file, "\nY:\n\t");
    for (int i = 0; i < ns->size; i++)
    {
        for (int j = 0; j < ns->size; j++)
        {
            fscanf(file, "%lf ", &ns->Y[i][j]);
        }
        fscanf(file, "\n\t");
    }

    fscanf(file, "\nV:\n\t");
    for (int i = 0; i < ns->size; i++)
    {
        fscanf(file, "%lf ", &ns->V[i]);
    }
    fscanf(file, "\n");

    fclose(file);

    return 0;
}

int deserialize_interface(interface* it, int id)
{
    char name[100], version[100];

    snprintf(name, 100, INTERFACE_FILE_NAME_FORMAT, id);
    FILE* file = fopen(name, "r");
    if (file == NULL)
    {
        printf("Impossible to open the network file");
        return -1;
    }

    fscanf(file, "IT FILE - VERSION %s", version);
    if (strcmp(version, VERSION_NUMBER) != 0)
    {
        printf("The file version is not supported by this version of the simulator");
        return -1;
    }

    fscanf(file, "\n\nmask size: %d", &it->mask_size);

    it->sources_mask = vector(bool, it->mask_size);
    it->grounds_mask = vector(bool, it->mask_size);
    it->loads_mask = vector(bool, it->mask_size);
    it->loads_weight = vector(double, it->mask_size);

    fscanf(file, "\nsources count: %d", &it->sources_count);
    for (int i = 0; i < it->mask_size; i++)
    {
        fscanf(file, "%d ", &it->sources_mask[i]);
    }
    fscanf(file, "\n");

    fscanf(file, "\ngrounds count: %d", &it->grounds_count);
    for (int i = 0; i < it->mask_size; i++)
    {
        fscanf(file, "%d ", &it->grounds_mask[i]);
    }
    fscanf(file, "\n");

    fscanf(file, "\nloads count: %d", &it->loads_count);
    for (int i = 0; i < it->mask_size; i++)
    {
        fscanf(file, "%d ", &it->loads_mask[i]);
    }
    fscanf(file, "\n");
    for (int i = 0; i < it->mask_size; i++)
    {
        fscanf(file, "%lf ", &it->loads_weight[i]);
    }
    fscanf(file, "\n");

    fclose(file);

    return 0;
}
