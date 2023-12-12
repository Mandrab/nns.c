#include <stdlib.h>
#include <string.h>

#include "device/component.h"
#include "util/tensors.h"

int cccmp(const void* e1, const void* e2) 
{
    connected_component a = *((connected_component*)e1);
    connected_component b = *((connected_component*)e2);

    if (a.ws_count == b.ws_count)
    {
        return a.js_count - b.js_count;
    }
    return a.ws_count - b.ws_count;
}

connected_component copy_component(connected_component cc)
{
    connected_component copy = (connected_component)
    {
        cc.ws_count,
        cc.js_count,
        cc.ws_skip,
        cc.js_skip,
        vector(int, cc.js_count)
    };

    // copy the Is data from the old to the new stucture
    memcpy(copy.Is, cc.Is, sizeof(int) * cc.js_count);

    return copy;
}

void destroy_component(connected_component cc)
{
    free(cc.Is);
}
