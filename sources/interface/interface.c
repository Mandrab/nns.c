#include <stdlib.h>
#include <string.h>

#include "interface/interface.h"
#include "util/tensors.h"

interface copy(const interface it)
{
    interface copy;
    copy.mask_size = it.mask_size;
    copy.sources_count = it.sources_count;
    copy.sources_mask = vector(bool, it.mask_size);
    copy.grounds_count = it.grounds_count;
    copy.grounds_mask = vector(bool, it.mask_size);
    copy.loads_count = it.loads_count;
    copy.loads_mask = vector(bool, it.mask_size);
    copy.loads_weight = vector(double, it.mask_size);

    // copy the memory area of the array of the original interface
    memcpy(copy.sources_mask, it.sources_mask, it.mask_size * sizeof(bool));
    memcpy(copy.grounds_mask, it.grounds_mask, it.mask_size * sizeof(bool));
    memcpy(copy.loads_mask, it.loads_mask, it.mask_size * sizeof(bool));
    memcpy(copy.loads_weight, it.loads_weight, it.mask_size * sizeof(double));

    return copy;
}

void destroy_interface(interface it)
{
    free(it.sources_mask);
    free(it.grounds_mask);
    free(it.loads_mask);
    free(it.loads_weight);
}
