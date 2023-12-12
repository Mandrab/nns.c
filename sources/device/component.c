#include "device/component.h"

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

void destroy_component(connected_component cc)
{
    free(cc.Is);
}
