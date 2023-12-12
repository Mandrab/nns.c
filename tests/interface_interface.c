#include "tests.h"
#include "config.h"
#include "interface/interface.h"
#include "util/errors.h"

void test_interface_comparison()
{
    interface ia = { };
    interface ib = { };

    // LESSER

    ib.sources_count += 2;
    int result = itcmp(&ia, &ib);
    assert(result == -2, -1, INT_ERROR, "itcmp", -2, result);
    ib.sources_count -= 2;

    ib.grounds_count += 3;
    result = itcmp(&ia, &ib);
    assert(result == -3, -1, INT_ERROR, "itcmp", -3, result);
    ib.grounds_count -= 3;

    ib.loads_count += 4;
    result = itcmp(&ia, &ib);
    assert(result == -4, -1, INT_ERROR, "itcmp", -4, result);
    ib.loads_count -= 4;

    // EQUAL

    result = itcmp(&ia, &ib);
    assert(result == 0, -1, INT_ERROR, "itcmp", 0, result);

    // GREATER

    ib.sources_count -= 2;
    result = itcmp(&ia, &ib);
    assert(result == 2, -1, INT_ERROR, "itcmp", 2, result);
    ib.sources_count += 2;

    ib.grounds_count -= 3;
    result = itcmp(&ia, &ib);
    assert(result == 3, -1, INT_ERROR, "itcmp", 3, result);
    ib.grounds_count += 3;

    ib.loads_count -= 4;
    result = itcmp(&ia, &ib);
    assert(result == 4, -1, INT_ERROR, "itcmp", 4, result);
    ib.loads_count = 4;
}

int interface_interface()
{
    test_interface_comparison();

    return 0;
}
