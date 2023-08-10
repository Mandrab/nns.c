#include <stdio.h>
#include <stdlib.h>

#include "io/deserializer.h"
#include "util/errors.h"
#include "util/tensors.h"
#include "config.h"

extern const int VERSION_NUMBER;

int deserialize_network(datasheet* ds, network_topology* nt, int id)
{
    char name[100];
    int version;

    // open the file with the name according to the format
    snprintf(name, 100, NETWORK_FILE_NAME_FORMAT, id);
    FILE* file = fopen(name, "rb");
    assert(file != NULL, -1, "Impossible to open the network file");

    // load the version of the serialized file
    fread(&version, sizeof(int), 1, file);
    if (version != VERSION_NUMBER)
    {
        printf("The file version is not supported by this version of the simulator");
        return -1;
    }

    // DATASHEET READING

    fread(&ds->wires_count, sizeof(int), 1, file);
    fread(&ds->length_mean, sizeof(double), 1, file);
    fread(&ds->length_std_dev, sizeof(double), 1, file);
    fread(&ds->package_size, sizeof(int), 1, file);
    fread(&ds->generation_seed, sizeof(int), 1, file);

    // TOPOLOGY READING

    // load number of junctions
    fread(&nt->js_count, sizeof(int), 1, file);

    // load the wires informations
    nt->Ws = vector(wire, ds->wires_count);
    fread(nt->Ws, sizeof(wire), ds->wires_count, file);

    // load the junctions informations
    nt->Js = vector(junction, nt->js_count);
    fread(nt->Js, sizeof(junction), nt->js_count, file);

    fclose(file);

    return 0;
}

int deserialize_state(network_state* ns, int id)
{
    char name[100];
    int version;

    // open the file with the name according to the format
    snprintf(name, 100, STATE_FILE_NAME_FORMAT, id);
    FILE* file = fopen(name, "rb");
    assert(file != NULL, -1, "Impossible to open the state file");

    // load the version of the serialized file
    fread(&version, sizeof(int), 1, file);
    if (version != VERSION_NUMBER)
    {
        printf("The file version is not supported by this version of the simulator");
        return -1;
    }

    // load the network size
    fread(&ns->size, sizeof(int), 1, file);

    // load the A matrix
    ns->A = zeros_matrix(bool, ns->size, ns->size);
    for (int i = 0; i < ns->size; i++)
    {
        fread(ns->A[i], sizeof(bool), ns->size, file);
    }

    // load the Y matrix
    ns->Y = zeros_matrix(double, ns->size, ns->size);
    for (int i = 0; i < ns->size; i++)
    {
        fread(ns->Y[i], sizeof(double), ns->size, file);
    }

    // load the V array
    ns->V = zeros_vector(double, ns->size);
    fread(ns->V, sizeof(double), ns->size, file);

    fclose(file);

    return 0;
}

int deserialize_interface(interface* it, int id)
{
    char name[100];
    int version;

    // open the file with the name according to the format
    snprintf(name, 100, INTERFACE_FILE_NAME_FORMAT, id);
    FILE* file = fopen(name, "rb");
    assert(file != NULL, -1, "Impossible to open the interface file");

    // load the version of the serialized file
    fread(&version, sizeof(int), 1, file);
    if (version != VERSION_NUMBER)
    {
        printf("The file version is not supported by this version of the simulator");
        return -1;
    }

    // load the masks size
    fread(&it->mask_size, sizeof(int), 1, file);

    // create the data-structures to contain the information
    it->sources_mask = vector(bool, it->mask_size);
    it->grounds_mask = vector(bool, it->mask_size);
    it->loads_mask = vector(bool, it->mask_size);
    it->loads_weight = vector(double, it->mask_size);

    // load the sources count and mask
    fread(&it->sources_count, sizeof(int), 1, file);
    fread(it->sources_mask, sizeof(bool), it->mask_size, file);

    // load the grounds count and mask
    fread(&it->grounds_count, sizeof(int), 1, file);
    fread(it->grounds_mask, sizeof(bool), it->mask_size, file);

    // load the loads count, mask and weight
    fread(&it->loads_count, sizeof(int), 1, file);
    fread(it->loads_mask, sizeof(bool), it->mask_size, file);
    fread(it->loads_weight, sizeof(double), it->mask_size, file);

    fclose(file);

    return 0;
}
