#include "tests.h"
#include "config.h"
#include "device/datasheet.h"
#include "util/errors.h"

void test_datasheet_comparison()
{
    datasheet ds_a = { 3, 0, 0, 2, 1 };
    datasheet ds_b = { 3, 0, 0, 2, 1 };

    // LESSER

    ds_b.wires_count += 2;
    int result = dscmp(&ds_a, &ds_b);
    assert(result == -2, -1, INT_ERROR, "dscmp", -2, result);
    ds_b.wires_count -= 2;

    ds_b.package_size += 3;
    result = dscmp(&ds_a, &ds_b);
    assert(result == -3, -1, INT_ERROR, "dscmp", -3, result);
    ds_b.package_size -= 3;

    ds_b.generation_seed += 4;
    result = dscmp(&ds_a, &ds_b);
    assert(result == -4, -1, INT_ERROR, "dscmp", -4, result);
    ds_b.generation_seed -= 4;

    // EQUAL

    result = dscmp(&ds_a, &ds_b);
    assert(result == 0, -1, INT_ERROR, "dscmp", 0, result);

    // GREATER

    ds_b.wires_count -= 2;
    result = dscmp(&ds_a, &ds_b);
    assert(result == 2, -1, INT_ERROR, "dscmp", 2, result);
    ds_b.wires_count += 2;

    ds_b.package_size -= 3;
    result = dscmp(&ds_a, &ds_b);
    assert(result == 3, -1, INT_ERROR, "dscmp", 3, result);
    ds_b.package_size += 3;

    ds_b.generation_seed -= 4;
    result = dscmp(&ds_a, &ds_b);
    assert(result == 4, -1, INT_ERROR, "dscmp", 4, result);
    ds_b.generation_seed = 4;
}

int device_datasheet()
{
    test_datasheet_comparison();

    return 0;
}
