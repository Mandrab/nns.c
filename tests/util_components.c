#include <math.h>

#include "tests.h"
#include "util/components.h"
#include "util/errors.h"
#include "util/tensors.h"

#define SAMPLES_COUNT  1000000
#define QUANTS_COUNT 100
#define ACCURACY 0.001      // 0.1 % accuracy

const point p = { -1, -1 };

void test_map_single_component()
{
    datasheet ds = { 3 };
    junction js[2] = {
        (junction) { 0, 1, p },
        (junction) { 0, 2, p }
    };
    network_topology nt = { NULL, 2, js };

    int mapping[3];
    int result = map_components(ds, nt, mapping);

    assert(result == 1, -1, INT_ERROR, "result of map_component", 1, result);

    assert(mapping[0] == 0, -1, INT_ERROR, "mapping[0]", 0, mapping[0]);
    assert(mapping[1] == 0, -1, INT_ERROR, "mapping[1]", 0, mapping[1]);
    assert(mapping[2] == 0, -1, INT_ERROR, "mapping[2]", 0, mapping[2]);
}

void test_map_multiple_components()
{
    datasheet ds = { 4 };
    junction js[1] = { (junction) { 1, 3, p } };
    network_topology nt = { NULL, 1, js };

    int mapping[4];
    int result = map_components(ds, nt, mapping);

    assert(result == 3, -1, INT_ERROR, "result of map_component", 3, result);

    assert(mapping[0] == 0, -1, INT_ERROR, "mapping[0]", 0, mapping[0]);
    assert(mapping[1] == 1, -1, INT_ERROR, "mapping[1]", 1, mapping[1]);
    assert(mapping[2] == 2, -1, INT_ERROR, "mapping[2]", 2, mapping[2]);
    assert(mapping[3] == 1, -1, INT_ERROR, "mapping[3]", 1, mapping[3]);
}

void test_group_nanowires()
{
    datasheet ds = { 5 };
    wire ws[5] = {
        (wire) { p, p, p, 1 },
        (wire) { p, p, p, 2 },
        (wire) { p, p, p, 3 },
        (wire) { p, p, p, 4 },
        (wire) { p, p, p, 5 }
    };
    junction js[2] = {
        (junction) { 1, 3, p },
        (junction) { 2, 4, p }
    };
    network_topology nt = { ws, 2, js };
    int n2c[5] = { 1, 0, 2, 0, 2 };

    group_nanowires(ds, nt, n2c, 3);

    assert(fabs(nt.Ws[0].length - 2) < ACCURACY, -1, DOUBLE_ERROR, "nt.Ws[0].length", 2.0, nt.Ws[0].length);
    assert(fabs(nt.Ws[1].length - 4) < ACCURACY, -1, DOUBLE_ERROR, "nt.Ws[1].length", 4.0, nt.Ws[1].length);
    assert(fabs(nt.Ws[2].length - 1) < ACCURACY, -1, DOUBLE_ERROR, "nt.Ws[2].length", 1.0, nt.Ws[2].length);
    assert(fabs(nt.Ws[3].length - 3) < ACCURACY, -1, DOUBLE_ERROR, "nt.Ws[3].length", 3.0, nt.Ws[3].length);
    assert(fabs(nt.Ws[4].length - 5) < ACCURACY, -1, DOUBLE_ERROR, "nt.Ws[4].length", 5.0, nt.Ws[4].length);

    assert(nt.Js[0].first_wire == 0, -1, INT_ERROR, "nt.Js[0].first_wire", 0, nt.Js[0].first_wire);
    assert(nt.Js[0].second_wire == 1, -1, INT_ERROR, "nt.Js[0].second_wire", 1, nt.Js[0].second_wire);
    assert(nt.Js[1].first_wire == 3, -1, INT_ERROR, "nt.Js[1].first_wire", 3, nt.Js[1].first_wire);
    assert(nt.Js[1].second_wire == 4, -1, INT_ERROR, "nt.Js[1].second_wire", 4, nt.Js[1].second_wire);

    assert(n2c[0] == 0, -1, INT_ERROR, "n2c[0]", 0, n2c[0]);
    assert(n2c[1] == 0, -1, INT_ERROR, "n2c[1]", 0, n2c[1]);
    assert(n2c[2] == 1, -1, INT_ERROR, "n2c[2]", 1, n2c[2]);
    assert(n2c[3] == 2, -1, INT_ERROR, "n2c[3]", 2, n2c[3]);
    assert(n2c[4] == 2, -1, INT_ERROR, "n2c[4]", 2, n2c[4]);
}

void test_split_single_component()
{
    junction js[2] = {
        (junction) { 0, 1, (point) { -1, -1 } },
        (junction) { 0, 2, (point) { -1, -1 } }
    };
    int Is[2] = { 1, 2 };
    double Ys[2] = { 0.1, 0.2 };
    double Vs[3] = { 0.01, 0.02, 0.03 };

    datasheet ds = { 3 };
    network_topology nt = { NULL, 2, js };
    network_state ns = { Is, Ys, Vs };
    int n2c[3] = { 0, 0, 0 };

    connected_component cc = split_components(ds, nt, ns, n2c, 1)[0];

    assert(cc.ws_count == 3, -1, INT_ERROR, "cc.ws_count", 3, cc.ws_count);
    assert(cc.js_count == 2, -1, INT_ERROR, "cc.js_count", 2, cc.js_count);

    assert(cc.ws_skip == 0, -1, INT_ERROR, "cc.ws_skip", 0, cc.ws_skip);
    assert(cc.js_skip == 0, -1, INT_ERROR, "cc.js_skip", 0, cc.js_skip);

    assert(cc.Is[0] == 1, -1, INT_ERROR, "cc.Is[0]", 1, cc.Is[0]);
    assert(cc.Is[1] == 2, -1, INT_ERROR, "cc.Is[1]", 2, cc.Is[1]);

    assert(fabs(ns.Ys[cc.js_skip + 0] - 0.1) < ACCURACY, -1, DOUBLE_ERROR, "ns.Ys[cc.js_skip + 0]", 0.1, ns.Ys[cc.js_skip + 0]);
    assert(fabs(ns.Ys[cc.js_skip + 1] - 0.2) < ACCURACY, -1, DOUBLE_ERROR, "ns.Ys[cc.js_skip + 1]", 0.2, ns.Ys[cc.js_skip + 1]);

    assert(fabs(ns.Vs[cc.ws_skip + 0] - 0.01) < ACCURACY, -1, DOUBLE_ERROR, "ns.Vs[cc.ws_skip + 0]", 0.01, ns.Vs[cc.ws_skip + 0]);
    assert(fabs(ns.Vs[cc.ws_skip + 1] - 0.02) < ACCURACY, -1, DOUBLE_ERROR, "ns.Vs[cc.ws_skip + 1]", 0.02, ns.Vs[cc.ws_skip + 1]);
    assert(fabs(ns.Vs[cc.ws_skip + 2] - 0.03) < ACCURACY, -1, DOUBLE_ERROR, "ns.Vs[cc.ws_skip + 2]", 0.03, ns.Vs[cc.ws_skip + 2]);
}

void test_split_multiple_components()
{
    junction js[3] = {
        (junction) { 0, 1, (point) { -1, -1 } },
        (junction) { 0, 2, (point) { -1, -1 } },
        (junction) { 4, 5, (point) { -1, -1 } }
    };
    int Is[3] = { 1, 2, 3 };
    double Ys[3] = { 0.1, 0.2, 0.3 };
    double Vs[6] = { 0.01, 0.02, 0.03, 0.04, 0.05, 0.06 };

    datasheet ds = { 6 };
    network_topology nt = { NULL, 3, js };
    network_state ns = { Is, Ys, Vs };
    int n2c[6] = { 0, 0, 1, 0, 2, 2 };

    connected_component* ccs = split_components(ds, nt, ns, n2c, 3);

    assert(ccs[0].ws_count == 3, -1, INT_ERROR, "ccs[0].ws_count", 3, ccs[0].ws_count);
    assert(ccs[1].ws_count == 1, -1, INT_ERROR, "ccs[1].ws_count", 1, ccs[1].ws_count);
    assert(ccs[2].ws_count == 2, -1, INT_ERROR, "ccs[2].ws_count", 2, ccs[2].ws_count);

    assert(ccs[0].js_count == 2, -1, INT_ERROR, "ccs[0].js_count", 2, ccs[0].js_count);
    assert(ccs[1].js_count == 0, -1, INT_ERROR, "ccs[1].js_count", 0, ccs[1].js_count);
    assert(ccs[2].js_count == 1, -1, INT_ERROR, "ccs[2].js_count", 1, ccs[2].js_count);

    assert(ccs[0].ws_skip == 0, -1, INT_ERROR, "ccs[0].ws_skip", 0, ccs[0].ws_skip);
    assert(ccs[1].ws_skip == 3, -1, INT_ERROR, "ccs[1].ws_skip", 3, ccs[1].ws_skip);
    assert(ccs[2].ws_skip == 4, -1, INT_ERROR, "ccs[2].ws_skip", 4, ccs[2].ws_skip);

    assert(ccs[0].js_skip == 0, -1, INT_ERROR, "ccs[0].js_skip", 0, ccs[0].js_skip);
    assert(ccs[1].js_skip == 2, -1, INT_ERROR, "ccs[1].js_skip", 2, ccs[1].js_skip);
    assert(ccs[2].js_skip == 2, -1, INT_ERROR, "ccs[2].js_skip", 2, ccs[2].js_skip);

    assert(ccs[0].Is[0] == 1, -1, INT_ERROR, "ccs[0].Is[0]", 1, ccs[0].Is[0]);
    assert(ccs[0].Is[1] == 2, -1, INT_ERROR, "ccs[0].Is[1]", 2, ccs[0].Is[1]);
    assert(ccs[1].Is == NULL, -1, INT_ERROR, "ccs[1].Is", NULL, ccs[1].Is);
    assert(ccs[2].Is[0] == 1, -1, INT_ERROR, "ccs[2].Is[0]", 1, ccs[2].Is[0]);
}

int util_components()
{
    test_map_single_component();
    test_map_multiple_components();
    test_group_nanowires();
    test_split_single_component();
    test_split_multiple_components();

    return 0;
}
