#include <stdlib.h>

#include "interface/interface.h"

void destroy_stack_interface(interface it)
{
    free(it.sources_mask);
    free(it.grounds_mask);
    free(it.loads_mask);
    free(it.loads_weight);
}

void destroy_heap_interface(interface* it)
{
    destroy_stack_interface(*it);
    free(it);
}
