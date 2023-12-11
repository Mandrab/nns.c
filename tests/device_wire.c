#include "tests.h"
#include "config.h"
#include "device/wire.h"
#include "util/errors.h"

void test_wire_comparison()
{
    wire wa = { };
    wire wb = { };

    // LESSER

    wb.length = 2;
    int result = wcmp(&wa, &wb);
    assert(result == -2, -1, INT_ERROR, "wcmp", -2, result);

    // EQUAL

    wb.length = 0;
    result = wcmp(&wa, &wb);
    assert(result == 0, -1, INT_ERROR, "wcmp", 0, result);

    // GREATER

    wb.length = -2;
    result = wcmp(&wa, &wb);
    assert(result == 2, -1, INT_ERROR, "wcmp", 2, result);
}

int device_wire()
{
    test_wire_comparison();

    return 0;
}
