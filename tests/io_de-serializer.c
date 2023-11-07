#include <math.h>

#include "io/serializer.h"
#include "io/deserializer.h"
#include "util/errors.h"
#include "tests.h"

#define TOLERANCE 1e-6

void test_network_io()
{
    const datasheet ds = {
        2000,
        14.0,
        14.0 * 0.35,
        200,
        1234
    };
    int n2c[2000], cc_count;
    const network_topology nt = create_network(ds, n2c, &cc_count);

    serialize_network(ds, nt, ".", 0);
    datasheet loaded_ds;
    network_topology loaded_nt;
    deserialize_network(&loaded_ds, &loaded_nt, ".", 0);

    assert(
        loaded_ds.wires_count == ds.wires_count, -1,
        "The (de)serialization of wires_count is not correct: original = %d, loaded = %d",
        ds.wires_count, loaded_ds.wires_count
    );
    assert(
        fabs(loaded_ds.length_mean - ds.length_mean) < TOLERANCE, -1,
        "The (de)serialization of length_mean is not correct: original = %f, loaded = %f",
        ds.length_mean, loaded_ds.length_mean
    );
    assert(
        fabs(loaded_ds.length_std_dev - ds.length_std_dev) < TOLERANCE, -1,
        "The (de)serialization of length_std_dev is not correct: original = %f, loaded = %f",
        ds.length_std_dev, loaded_ds.length_std_dev
    );
    assert(
        loaded_ds.package_size == ds.package_size, -1,
        "The (de)serialization of package_size is not correct: original = %d, loaded = %d",
        ds.package_size, loaded_ds.package_size
    );
    assert(
        loaded_ds.generation_seed == ds.generation_seed, -1,
        "The (de)serialization of generation_seed is not correct: original = %d, loaded = %d",
        ds.generation_seed, loaded_ds.generation_seed
    );

    for (int i = 0; i < ds.wires_count; i++)
    {
        assert(
            fabs(loaded_nt.Ws[i].centroid.x - nt.Ws[i].centroid.x) < TOLERANCE, -1,
            "The (de)serialization of centroid.x is not correct"
        );
        assert(
            fabs(loaded_nt.Ws[i].centroid.y - nt.Ws[i].centroid.y) < TOLERANCE, -1,
            "The (de)serialization of centroid.y is not correct"
        );
        assert(
            fabs(loaded_nt.Ws[i].start_edge.x - nt.Ws[i].start_edge.x) < TOLERANCE, -1,
            "The (de)serialization of start_edge.x is not correct"
        );
        assert(
            fabs(loaded_nt.Ws[i].start_edge.y - nt.Ws[i].start_edge.y) < TOLERANCE, -1,
            "The (de)serialization of start_edge.y is not correct"
        );
        assert(
            fabs(loaded_nt.Ws[i].end_edge.x - nt.Ws[i].end_edge.x) < TOLERANCE, -1,
            "The (de)serialization of end_edge.x is not correct"
        );
        assert(
            fabs(loaded_nt.Ws[i].end_edge.y - nt.Ws[i].end_edge.y) < TOLERANCE, -1,
            "The (de)serialization of end_edge.y is not correct"
        );
        assert(
            fabs(loaded_nt.Ws[i].length - nt.Ws[i].length) < TOLERANCE, -1,
            "The (de)serialization of length is not correct"
        );
    }
    assert(
        loaded_nt.js_count == nt.js_count, -1,
        "The (de)serialization of js_count is not correct"
    );
    for (int i = 0; i < nt.js_count; i++)
    {
        assert(
            loaded_nt.Js[i].first_wire == nt.Js[i].first_wire, -1,
            "The (de)serialization of first_wire is not correct"
        );
        assert(
            loaded_nt.Js[i].second_wire == nt.Js[i].second_wire, -1,
            "The (de)serialization of second_wire is not correct"
        );
        assert(
            fabs(loaded_nt.Js[i].position.x - nt.Js[i].position.x) < TOLERANCE, -1,
            "The (de)serialization of position.x is not correct"
        );
        assert(
            fabs(loaded_nt.Js[i].position.y - nt.Js[i].position.y) < TOLERANCE, -1,
            "The (de)serialization of position.y is not correct"
        );
    }
}

void test_state_io()
{
    const datasheet ds = {
        2000,
        14.0,
        14.0 * 0.35,
        200,
        1234
    };
    int n2c[2000], cc_count;
    const network_topology nt = create_network(ds, n2c, &cc_count);
    const network_state ns = construe_circuit(ds, nt);

    serialize_state(ds, nt, ns, ".", 0, 0);
    network_state loaded_ns;
    deserialize_state(ds, nt, &loaded_ns, ".", 0, 0);

    for (int i = 0; i < ds.wires_count; i++)
    {
        assert(fabs(ns.Vs[i] - loaded_ns.Vs[i]) < TOLERANCE, -1, DOUBLE_ERROR, "loaded_ns.Vs[i]", ns.Vs[i], loaded_ns.Vs[i]);
    }

    for (int i = 0; i < nt.js_count; i++)
    {
        assert(fabs(ns.Ys[i] - loaded_ns.Ys[i]) < TOLERANCE, -1, DOUBLE_ERROR, "loaded_ns.Ys[i]", ns.Ys[i], loaded_ns.Ys[i]);
    }
}

void test_component_io()
{
    int Is[2] = { 5, 6 };
    connected_component cc = { 1, 2, 3, 4, Is };
    connected_component loaded_cc;

    serialize_component(cc, ".", 0, 1);
    deserialize_component(&loaded_cc, ".", 0, 1);

    // check that the original cc is not modified by the function
    assert(cc.ws_count == 1, -1, INT_ERROR, "cc.ws_count", 1, cc.ws_count);
    assert(cc.js_count == 2, -1, INT_ERROR, "cc.js_count", 2, cc.js_count);
    assert(cc.ws_skip == 3, -1,  INT_ERROR, "cc.ws_skip",  3, cc.ws_skip);
    assert(cc.js_skip == 4, -1,  INT_ERROR, "cc.js_skip",  4, cc.js_skip);

    assert(cc.Is[0] == 5, -1, INT_ERROR, "cc.Is[0]", 5, cc.Is[0]);
    assert(cc.Is[1] == 6, -1, INT_ERROR, "cc.Is[1]", 6, cc.Is[1]);

    // check that the loading is performed correctly
    assert(loaded_cc.ws_count == 1, -1, INT_ERROR, "loaded_cc.ws_count", 1, loaded_cc.ws_count);
    assert(loaded_cc.js_count == 2, -1, INT_ERROR, "loaded_cc.js_count", 2, loaded_cc.js_count);
    assert(loaded_cc.ws_skip == 3, -1,  INT_ERROR, "loaded_cc.ws_skip",  3, loaded_cc.ws_skip);
    assert(loaded_cc.js_skip == 4, -1,  INT_ERROR, "loaded_cc.js_skip",  4, loaded_cc.js_skip);

    assert(loaded_cc.Is[0] == 5, -1, INT_ERROR, "loaded_cc.Is[0]", 5, loaded_cc.Is[0]);
    assert(loaded_cc.Is[1] == 6, -1, INT_ERROR, "loaded_cc.Is[1]", 6, loaded_cc.Is[1]);
}

void test_interface_io()
{
    int S[2] = { 0, 3 };
    int G[2] = { 1, 4  };
    int L[1] = { 5 };
    double W[1] = { .3 };
    const interface it = { 2, S, 2, G, 1, L, W };

    serialize_interface(it, ".", 0, 0);
    interface loaded_it;
    deserialize_interface(&loaded_it, ".", 0, 0);

    assert(
        loaded_it.sources_count == it.sources_count, -1,
        "The (de)serialization of sources_count is not correct: original = %d, loaded = %d",
        it.sources_count, loaded_it.sources_count
    );
    for (int i = 0; i < it.sources_count; i++)
    {
        assert(
            loaded_it.sources_index[i] == it.sources_index[i], -1,
            "The (de)serialization of sources_index[%d] is not correct: original = %d, loaded = %d",
            i, it.sources_index[i], loaded_it.sources_index[i]
        );
    }
    assert(
        loaded_it.grounds_count == it.grounds_count, -1,
        "The (de)serialization of grounds_count is not correct: original = %d, loaded = %d",
        it.grounds_count, loaded_it.grounds_count
    );
    for (int i = 0; i < it.grounds_count; i++)
    {
        assert(
            loaded_it.grounds_index[i] == it.grounds_index[i], -1,
            "The (de)serialization of grounds_index[%d] is not correct: original = %d, loaded = %d",
            i, it.grounds_index[i], loaded_it.grounds_index[i]
        );
    }
    assert(
        loaded_it.loads_count == it.loads_count, -1,
        "The (de)serialization of loads_count is not correct: original = %d, loaded = %d",
        it.loads_count, loaded_it.loads_count
    );
    for (int i = 0; i < it.loads_count; i++)
    {
        assert(
            loaded_it.loads_index[i] == it.loads_index[i], -1,
            "The (de)serialization of loads_index[%d] is not correct: original = %d, loaded = %d",
            i, it.loads_index[i], loaded_it.loads_index[i]
        );
        assert(
            fabs(loaded_it.loads_weight[i] - it.loads_weight[i]) < TOLERANCE, -1,
            "The (de)serialization of loads_weight[%d] is not correct: original = %f, loaded = %f",
            i, it.loads_weight[i], loaded_it.loads_weight[i]
        );
    }
}

int io_de_serializer()
{
    test_network_io();
    test_state_io();
    test_component_io();
    test_interface_io();

    return 0;
}
