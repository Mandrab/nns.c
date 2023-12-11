#include "tests.h"
#include "config.h"
#include "device/network.h"
#include "util/errors.h"

void test_construe_circuit()
{
    datasheet ds = { 3, 0, 0, 0, 0 };
    junction js[2] = {
        (junction) { 0, 1, (point) { -1, -1 } },
        (junction) { 1, 2, (point) { -1, -1 } }
    };
    network_topology nt = { NULL, 2, js };

    network_state ns = construe_circuit(ds, nt);

    assert(ns.Ys[0] == Y_MIN, -1, DOUBLE_ERROR, "ns.Ys[0]", Y_MIN, ns.Ys[0]);
    assert(ns.Ys[1] == Y_MIN, -1, DOUBLE_ERROR, "ns.Ys[1]", Y_MIN, ns.Ys[1]);

    assert(ns.Vs[0] == 0, -1, DOUBLE_ERROR, "ns.Vs[0]", .0, ns.Vs[0]);
    assert(ns.Vs[1] == 0, -1, DOUBLE_ERROR, "ns.Vs[1]", .0, ns.Vs[1]);
}

void test_nt_comparison()
{
    network_topology nt_a = { };
    network_topology nt_b = { };

    // LESSER

    nt_b.js_count = 2;
    int result = ntcmp(&nt_a, &nt_b);
    assert(result == -2, -1, INT_ERROR, "ntcmp", -2, result);

    // EQUAL

    nt_b.js_count = 0;
    result = ntcmp(&nt_a, &nt_b);
    assert(result == 0, -1, INT_ERROR, "ntcmp", 0, result);

    // GREATER

    nt_b.js_count = -2;
    result = ntcmp(&nt_a, &nt_b);
    assert(result == 2, -1, INT_ERROR, "ntcmp", 2, result);
}

int device_network()
{
    test_construe_circuit();
    test_nt_comparison();

    return 0;
}
