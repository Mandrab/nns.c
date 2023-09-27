#include <math.h>

#include "io/serializer.h"
#include "io/deserializer.h"
#include "util/errors.h"

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
    const network_topology nt = create_network(ds);

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
    const network_topology nt = create_network(ds);
    const network_state ns = construe_circuit(ds, nt);

    serialize_state(ns, ".", 0, 0);
    network_state loaded_ns;
    deserialize_state(&loaded_ns, ".", 0, 0);

    assert(
        loaded_ns.size == ns.size, -1,
        "The (de)serialization of size is not correct: original = %d, loaded = %d",
        ns.size, loaded_ns.size
    );
    
    for (int i = 0; i < ns.size; i++)
    {
        for (int j = 0; j < ns.size; j++)
        {
            assert(
                loaded_ns.A[i][j] == ns.A[i][j], -1,
                "The (de)serialization of A[%d][%d] is not correct: original = %d, loaded = %d",
                i, j, ns.A[i][j], loaded_ns.A[i][j]
            );
            assert(
                fabs(loaded_ns.Y[i][j] - ns.Y[i][j]) < TOLERANCE, -1,
                "The (de)serialization of Y[%d][%d] is not correct: original = %f, loaded = %f",
                i, j, ns.Y[i][j], loaded_ns.Y[i][j]
            );
        }
        assert(
            fabs(loaded_ns.V[i] - ns.V[i]) < TOLERANCE, -1,
            "The (de)serialization of V[%d] is not correct: original = %f, loaded = %f",
            i, ns.V[i], loaded_ns.V[i]
        );
    }
}

void test_interface_io()
{
    bool S[5] = { false, true,  false, true,  false };
    bool G[5] = { true,  false, false, false, true  };
    bool L[5] = { false, false, true,  false, false };
    double W[5] = { .1, .2, .3, .4, .5 };
    const interface it = { 5, 2, S, 2, G, 1, L, W };

    serialize_interface(it, ".", 0, 0);
    interface loaded_it;
    deserialize_interface(&loaded_it, ".", 0, 0);

    assert(
        loaded_it.mask_size == it.mask_size, -1,
        "The (de)serialization of mask_size is not correct: original = %d, loaded = %d",
        it.mask_size, loaded_it.mask_size
    );
    assert(
        loaded_it.sources_count == it.sources_count, -1,
        "The (de)serialization of sources_count is not correct: original = %d, loaded = %d",
        it.sources_count, loaded_it.sources_count
    );
    for (int i = 0; i < it.mask_size; i++)
    {
        assert(
            loaded_it.sources_mask[i] == it.sources_mask[i], -1,
            "The (de)serialization of sources_mask[%d] is not correct: original = %d, loaded = %d",
            i, it.sources_mask[i], loaded_it.sources_mask[i]
        );
    }
    assert(
        loaded_it.grounds_count == it.grounds_count, -1,
        "The (de)serialization of grounds_count is not correct: original = %d, loaded = %d",
        it.grounds_count, loaded_it.grounds_count
    );
    for (int i = 0; i < it.mask_size; i++)
    {
        assert(
            loaded_it.grounds_mask[i] == it.grounds_mask[i], -1,
            "The (de)serialization of grounds_mask[%d] is not correct: original = %d, loaded = %d",
            i, it.grounds_mask[i], loaded_it.grounds_mask[i]
        );
    }
    assert(
        loaded_it.loads_count == it.loads_count, -1,
        "The (de)serialization of loads_count is not correct: original = %d, loaded = %d",
        it.loads_count, loaded_it.loads_count
    );
    for (int i = 0; i < it.mask_size; i++)
    {
        assert(
            loaded_it.loads_mask[i] == it.loads_mask[i], -1,
            "The (de)serialization of loads_mask[%d] is not correct: original = %d, loaded = %d",
            i, it.loads_mask[i], loaded_it.loads_mask[i]
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
    test_interface_io();

    return 0;
}
