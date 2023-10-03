#include "device/junction.h"

int jcmp(const void* e1, const void* e2)
{
    junction a = *((junction*) e1);
    junction b = *((junction*) e2);

    if (a.first_wire == b.first_wire)
    {
        return a.second_wire - b.second_wire;
    }
    return a.first_wire - b.first_wire;
}
