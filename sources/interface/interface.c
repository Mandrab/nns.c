#include <stdlib.h>
#include <string.h>

#include "interface/interface.h"
#include "util/tensors.h"

interface copy(const interface it)
{
    interface copy = (interface)
    {
        it.sources_count,
        vector(int, it.sources_count),

        it.grounds_count,
        vector(int, it.grounds_count),

        it.loads_count,
        vector(int, it.loads_count),
        vector(double, it.loads_count)
    };

    // copy the memory area of the array of the original interface
    memcpy(copy.sources_index, it.sources_index, sizeof(int) * it.sources_count);
    memcpy(copy.grounds_index, it.grounds_index, sizeof(int) * it.grounds_count);
    memcpy(copy.loads_index,   it.loads_index,   sizeof(int) * it.loads_count);
    memcpy(copy.loads_weight,  it.loads_weight,  sizeof(double) * it.loads_count);

    return copy;
}

void destroy_interface(interface it)
{
    free(it.sources_index);
    free(it.grounds_index);
    free(it.loads_index);
    free(it.loads_weight);
}
