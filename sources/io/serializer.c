#include <stdio.h>
#include <sys/stat.h>

#include "interface/connection.h"
#include "io/serializer.h"
#include "util/errors.h"
#include "config.h"

const int VERSION_NUMBER = 1;

// create and open a file with index `e_id' in a folder with index `nn_id'; if
// the folder/file do not exist, create the folder/path/file
FILE* new_file(char* file_type, char* path, int nn_id, int e_id);

void serialize_network(
    const datasheet ds,
    const network_topology nt,
    char* path,
    int id
)
{
    // create and open folder and file of the specific format
    FILE* file = new_file(NETWORK_FILE_NAME_FORMAT, path, id, -1);

    // DATASHEET WRITING

    fwrite(&ds.wires_count,     sizeof(int),    1, file);
    fwrite(&ds.length_mean,     sizeof(double), 1, file);
    fwrite(&ds.length_std_dev,  sizeof(double), 1, file);
    fwrite(&ds.package_size,    sizeof(int),    1, file);
    fwrite(&ds.generation_seed, sizeof(int),    1, file);

    // TOPOLOGY WRITING

    fwrite(&nt.js_count, sizeof(int),      1,              file);
    fwrite(nt.Ws,        sizeof(wire),     ds.wires_count, file);
    fwrite(nt.Js,        sizeof(junction), nt.js_count,    file);

    fclose(file);
}

void serialize_state(
    const datasheet ds,
    const network_topology nt,
    const network_state ns,
    char* path,
    int id,
    int step
)
{
    // create and open folder and file of the specific format
    FILE* file = new_file(STATE_FILE_NAME_FORMAT, path, id, step);

    // write the junctions weight and the nodes voltage
    fwrite(ns.Ys, sizeof(double), nt.js_count,    file);
    fwrite(ns.Vs, sizeof(double), ds.wires_count, file);

    fclose(file);
}

void serialize_component(
    const connected_component cc,
    char* path,
    int nn_id,
    int cc_id
)
{
    // create and open folder and file of the specific format
    FILE* file = new_file(COMPONENT_FILE_NAME_FORMAT, path, nn_id, cc_id);

    fwrite(&cc,   sizeof(int), 4,           file);
    fwrite(cc.Is, sizeof(int), cc.js_count, file);

    fclose(file);
}

void serialize_interface(const interface it, char* path, int id, int step)
{
    // create and open folder and file of the specific format
    FILE* file = new_file(INTERFACE_FILE_NAME_FORMAT, path, id, step);

    // write the sources, grounds and loads count and mask; write also the
    // loads weights
    fwrite(&it.sources_count, sizeof(int),    1,                file);
    fwrite(it.sources_index,  sizeof(int),    it.sources_count, file);
    fwrite(&it.grounds_count, sizeof(int),    1,                file);
    fwrite(it.grounds_index,  sizeof(int),    it.grounds_count, file);
    fwrite(&it.loads_count,   sizeof(int),    1,                file);
    fwrite(it.loads_index,    sizeof(int),    it.loads_count,   file);
    fwrite(it.loads_weight,   sizeof(double), it.loads_count,   file);

    fclose(file);
}

void serialize_mea(const MEA mea, char* path, int id, int step)
{
    // create and open folder and file of the specific format
    FILE* file = new_file(MEA_FILE_NAME_FORMAT, path, id, step);

    // write the mea arrays to file
    fwrite(mea.Ps,  sizeof(point),          MEA_ELECTRODES, file);
    fwrite(mea.e2n, sizeof(int),            MEA_ELECTRODES, file);
    fwrite(mea.ct,  sizeof(connection_t),   MEA_ELECTRODES, file);
    fwrite(mea.ws,  sizeof(double),         MEA_ELECTRODES, file);

    fclose(file);
}

FILE* new_file(char* file_type, char* path, int nn_id, int e_id)
{
    char name[100];

    // create the name of the folder according to the format
    snprintf(name, 100, DIRECTORY_FORMAT, path, nn_id);
    mkdir(name, 0700);

    // create the name of the file according to the format
    snprintf(name, 100, file_type, path, nn_id, e_id);

    // open the file and check that it correctly opened
    FILE* file = fopen(name, "wb");
    assert(file != NULL, -1, "Impossible to open file: %s for writing operations\n", name);

    // write the version of the serialized file
    fwrite(&VERSION_NUMBER, sizeof(int), 1, file);

    return file;
}
