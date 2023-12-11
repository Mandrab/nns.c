#include "device/wire.h"

int wcmp(const void* e1, const void* e2)
{
    wire a = *((wire*) e1);
    wire b = *((wire*) e2);

    return a.length - b.length;
}
