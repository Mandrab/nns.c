#include "device/datasheet.h"

int dscmp(const void* e1, const void* e2)
{
    datasheet a = *((datasheet*)e1);
    datasheet b = *((datasheet*)e2);

    int result = a.wires_count - b.wires_count;

    if (result == 0)
    {
        result = a.package_size - b.package_size;
    }

    if (result == 0)
    {
        result = a.generation_seed - b.generation_seed;
    }

    return result;
}
