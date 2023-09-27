#include <stdio.h>
#include <stdlib.h>

#include "io/deserializer.h"
#include "util/errors.h"
#include "util/tensors.h"
#include "config.h"

extern const int VERSION_NUMBER;

// open the file and check the version
FILE* open_file(char* file_type, char* path, int id, int step);

void deserialize_network(datasheet* ds, network_topology* nt, char* path, int id)
{
    // open the file and check the version
    FILE* file = open_file(NETWORK_FILE_NAME_FORMAT, path, id, -1);

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
}

void deserialize_state(network_state* ns, char* path, int id, int step)
{
    // open the file and check the version
    FILE* file = open_file(STATE_FILE_NAME_FORMAT, path, id, step);

    // load the network size
    fread(&ns->size, sizeof(int), 1, file);

    // load the number of junctions
    int js_counter;
    fread(&js_counter, sizeof(int), 1, file);

    // create a buffer to contain the junctions index and weight
    int I[js_counter];
    double Y[js_counter];

    // load the junctions index and weight
    fread(I, sizeof(int), js_counter, file);
    fread(Y, sizeof(double), js_counter, file);

    // initialize and fill the the I matrix
    ns->A = zeros_matrix(bool, ns->size, ns->size);
    ns->Y = zeros_matrix(double, ns->size, ns->size);
    for (int i = 0; i < js_counter; i++)
    {
        // obtain the linearized index from I
        int j = I[i];
        ns->A[j / ns->size][j % ns->size] = true;
        ns->Y[j / ns->size][j % ns->size] = Y[i];
    }

    // load the V array
    ns->V = zeros_vector(double, ns->size);
    fread(ns->V, sizeof(double), ns->size, file);

    fclose(file);
}

void deserialize_interface(interface* it, char* path, int id, int step)
{
    // open the file and check the version
    FILE* file = open_file(INTERFACE_FILE_NAME_FORMAT, path, id, step);

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
}

FILE* open_file(char* file_type, char* path, int id, int step)
{
    char name[100];
    int version;

    // open the file with the name according to the format and check that it
    // opened correctly
    snprintf(name, 100, file_type, path, id, step);
    FILE* file = fopen(name, "rb");
    assert(file != NULL, -1, "Impossible to open file: %s for reading operations\n", name);

    // load the version of the serialized file
    fread(&version, sizeof(int), 1, file);
    assert(version == VERSION_NUMBER, -1, "The file version is not supported by this version of the simulator\n");

    return file;
}
