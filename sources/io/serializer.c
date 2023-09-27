#include <stdio.h>
#include <sys/stat.h>

#include "io/serializer.h"
#include "util/errors.h"
#include "config.h"

const int VERSION_NUMBER = 1;

// create and open a file; if it does not exists, create the folder/path
FILE* new_file(char* file_type, char* path, int id, int step);

void serialize_network(const datasheet ds, const network_topology nt, char* path, int id)
{
    // create and open folder and file of the specific format
    FILE* file = new_file(NETWORK_FILE_NAME_FORMAT, path, id, -1);

    // DATASHEET WRITING

    fwrite(&ds.wires_count, sizeof(int), 1, file);
    fwrite(&ds.length_mean, sizeof(double), 1, file);
    fwrite(&ds.length_std_dev, sizeof(double), 1, file);
    fwrite(&ds.package_size, sizeof(int), 1, file);
    fwrite(&ds.generation_seed, sizeof(int), 1, file);

    // TOPOLOGY WRITING

    fwrite(&nt.js_count, sizeof(int), 1, file);
    fwrite(nt.Ws, sizeof(wire), ds.wires_count, file);
    fwrite(nt.Js, sizeof(junction), nt.js_count, file);

    fclose(file);
}

void serialize_state(const network_state ns, char* path, int id, int step)
{
    // create and open folder and file of the specific format
    FILE* file = new_file(STATE_FILE_NAME_FORMAT, path, id, step);

    // create a buffer to contain the junction indexes and weight. this cannot
    // be larger than the size of the matrix, but can be shorter; create a
    // counter to memorize the number of elements in the buffer
    int I[ns.size * ns.size];
    double Y[ns.size * ns.size];
    int counter = 0;

    // fill the I and Y arrays and count the number of occurrences
    for (int i = 0; i < ns.size; i++)
    {
        for (int j = 0; j < ns.size; j++)
        {
            if (ns.A[i][j])
            {
                I[counter] = i * ns.size + j;
                Y[counter] = ns.Y[i][j];
                counter++;
            }
        }
    }

    // write the network size, the number of junctions, their
    // index, weight and the voltage value of each nanowire
    fwrite(&ns.size, sizeof(int), 1, file);
    fwrite(&counter, sizeof(int), 1, file);
    fwrite(I, sizeof(int), counter, file);
    fwrite(Y, sizeof(double), counter, file);
    fwrite(ns.V, sizeof(double), ns.size, file);

    fclose(file);
}

void serialize_interface(const interface it, char* path, int id, int step)
{
    // create and open folder and file of the specific format
    FILE* file = new_file(INTERFACE_FILE_NAME_FORMAT, path, id, step);

    // write the masks size
    fwrite(&it.mask_size, sizeof(int), 1, file);

    // write the sources, grounds and loads count and mask; write also the
    // loads weights
    fwrite(&it.sources_count, sizeof(int), 1, file);
    fwrite(it.sources_mask, sizeof(bool), it.mask_size, file);
    fwrite(&it.grounds_count, sizeof(int), 1, file);
    fwrite(it.grounds_mask, sizeof(bool), it.mask_size, file);
    fwrite(&it.loads_count, sizeof(int), 1, file);
    fwrite(it.loads_mask, sizeof(bool), it.mask_size, file);
    fwrite(it.loads_weight, sizeof(double), it.mask_size, file);

    fclose(file);
}

FILE* new_file(char* file_type, char* path, int id, int step)
{
    char name[100];

    // create the name of the folder according to the format
    snprintf(name, 100, DIRECTORY_FORMAT, path, id);
    mkdir(name, 0700);

    // create the name of the file according to the format
    snprintf(name, 100, file_type, path, id, step);

    // open the file and check that it correctly opened
    FILE* file = fopen(name, "wb");
    assert(file != NULL, -1, "Impossible to open file: %s for writing operations\n", name);

    // write the version of the serialized file
    fwrite(&VERSION_NUMBER, sizeof(int), 1, file);

    return file;
}
