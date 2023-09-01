#include <stdio.h>
#include <sys/stat.h>

#include "io/serializer.h"
#include "util/errors.h"
#include "config.h"

const int VERSION_NUMBER = 0;

int serialize_network(const datasheet ds, const network_topology nt, char* path, int id)
{
    char name[100];

    // create the name of the folder according to the format
    snprintf(name, 100, DIRECTORY_FORMAT, path, id);
    mkdir(name, 0700);

    // create the name of the file according to the format
    snprintf(name, 100, NETWORK_FILE_NAME_FORMAT, path, id);
    FILE* file = fopen(name, "wb");
    assert(file != NULL, -1, "Impossible to open the network file\n");

    // write the version of the serialized file
    fwrite(&VERSION_NUMBER, sizeof(int), 1, file);

    // DATASHEET WRITING

    fwrite(&ds.wires_count, sizeof(int), 1, file);
    fwrite(&ds.length_mean, sizeof(double), 1, file);
    fwrite(&ds.length_std_dev, sizeof(double), 1, file);
    fwrite(&ds.package_size, sizeof(int), 1, file);
    fwrite(&ds.generation_seed, sizeof(int), 1, file);

    // TOPOLOGY WRITING

    // write number of junctions
    fwrite(&nt.js_count, sizeof(int), 1, file);

    // write the wires and junctions informations
    fwrite(nt.Ws, sizeof(wire), ds.wires_count, file);
    fwrite(nt.Js, sizeof(junction), nt.js_count, file);

    fclose(file);

    return 0;
}

int serialize_state(const network_state ns, char* path, int id, int step)
{
    char name[100];

    // create the name of the folder according to the format
    snprintf(name, 100, DIRECTORY_FORMAT, path, id);
    mkdir(name, 0700);

    // create the name of the file according to the format
    snprintf(name, 100, STATE_FILE_NAME_FORMAT, path, id, step);
    FILE* file = fopen(name, "wb");
    assert(file != NULL, -1, "Impossible to open the state file\n");

    // write the version of the serialized file
    fwrite(&VERSION_NUMBER, sizeof(int), 1, file);

    // write the network size
    fwrite(&ns.size, sizeof(int), 1, file);

    // write the A matrix
    for (int i = 0; i < ns.size; i++)
    {
        fwrite(ns.A[i], sizeof(bool), ns.size, file);
    }

    // write the Y matrix
    for (int i = 0; i < ns.size; i++)
    {
        fwrite(ns.Y[i], sizeof(double), ns.size, file);
    }

    // write the V array
    fwrite(ns.V, sizeof(double), ns.size, file);

    fclose(file);

    return 0;
}

int serialize_interface(const interface it, char* path, int id, int step)
{
    char name[100];

    // create the name of the folder according to the format
    snprintf(name, 100, DIRECTORY_FORMAT, path, id);
    mkdir(name, 0700);

    // create the name of the file according to the format
    snprintf(name, 100, INTERFACE_FILE_NAME_FORMAT, path, id, step);
    FILE* file = fopen(name, "wb");
    assert(file != NULL, -1, "Impossible to open the interface file\n");

    // write the version of the serialized file
    fwrite(&VERSION_NUMBER, sizeof(int), 1, file);

    // write the masks size
    fwrite(&it.mask_size, sizeof(int), 1, file);

    // write the sources count and mask
    fwrite(&it.sources_count, sizeof(int), 1, file);
    fwrite(it.sources_mask, sizeof(bool), it.mask_size, file);

    // write the grounds count and mask
    fwrite(&it.grounds_count, sizeof(int), 1, file);
    fwrite(it.grounds_mask, sizeof(bool), it.mask_size, file);

    // write the loads count, mask and weight
    fwrite(&it.loads_count, sizeof(int), 1, file);
    fwrite(it.loads_mask, sizeof(bool), it.mask_size, file);
    fwrite(it.loads_weight, sizeof(double), it.mask_size, file);

    fclose(file);

    return 0;
}
