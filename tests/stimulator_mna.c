#include <math.h>

#include "device/network.h"
#include "stimulator/mna.h"
#include "tests.h"
#include "util/errors.h"
#include "util/tensors.h"

// allows 0.05 volt difference between calculated and expected
#define TOLERANCE 5e-2

/**
 * Testing the following circuit:
 * 
 *      SRC-----R-----R-----GND
 */
void test_divider_one()
{
    double nsYs[2] = { 1, 1 };
    double nsVs[3];
    int ccIs[2] = { 1, 5 };

    network_state ns = { nsYs, nsVs };
    connected_component cc = { 3, 2, 0, 0, ccIs };

    bool sources[3] = { true, false, false };
    bool grounds[3] = { false, false, true };
    bool loads[3] = { false, false, false };
    double weights[3] = { 0.0 };
    interface it = (interface) {
        3,
        1, sources,
        1, grounds,
        0, loads, weights
    };

    double vs[3] = { 5 };

    voltage_stimulation(ns, cc, it, vs);

    assert(fabs(ns.Vs[0] - 5.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[0]", 5.00, ns.Vs[0]); // a - source
    assert(fabs(ns.Vs[1] - 2.50) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[1]", 2.50, ns.Vs[1]); // b - voltage divider
    assert(fabs(ns.Vs[2] - 0.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[2]", 0.00, ns.Vs[2]); // c - ground
}

/**
 * Testing the following circuit:
 * 
 *      GND-----R-----SRC-----R-----GND
 *                     |
 *                     R
 *                     |
 *                    --
 */
void test_divider_two()
{
    double nsYs[3] = { 1, 1, 1 };
    double nsVs[4];
    int ccIs[3] = { 1, 6, 7 };

    network_state ns = { nsYs, nsVs };
    connected_component cc = { 4, 3, 0, 0, ccIs };

    bool sources[4] = { false, true, false, false };
    bool grounds[4] = { true, false, true, false };
    bool loads[4] = { false, false, false, false };
    interface it = (interface) {
        4,
        1, sources,
        2, grounds,
        0, loads, NULL
    };

    double vs[4] = { 0, 5 };

    voltage_stimulation(ns, cc, it, vs);

    assert(fabs(ns.Vs[0] - 0.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[0]", 0.00, ns.Vs[0]); // a - ground
    assert(fabs(ns.Vs[1] - 5.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[1]", 5.00, ns.Vs[1]); // b - source
    assert(fabs(ns.Vs[2] - 0.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[2]", 0.00, ns.Vs[2]); // c - ground
    assert(fabs(ns.Vs[3] - 5.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[3]", 5.00, ns.Vs[3]); // d - disconnected
}

/**
 * Testing the following circuit:
 * 
 *              SRC
 *               |
 *        --R_1-----R_2--
 *       |              |
 *      --             R_3
 *                      |
 *                     GND
 * 
 * where:
 * - R_1 = 1.00 Ω
 * - R_2 = 0.33 Ω
 * - R_3 = 0.25 Ω
 */
void test_parallel()
{
    // Ax = b -> A:
    //
    //     -------------------------
    //     |       b + 3c      | a |
    //     |  a                | b |
    //     | 3a           + 4d | c |
    //     |           4c      | d |
    //     -------------------------
    double nsYs[3] = { 1, 3, 4 };
    double nsVs[4];
    int ccIs[3] = { 1, 2, 11 };

    network_state ns = { nsYs, nsVs };
    connected_component cc = { 4, 3, 0, 0, ccIs };

    bool sources[4] = { true, false, false, false };
    bool grounds[4] = { false, false, false, true };
    bool loads[4] = { false, false, false, false };
    double weights[4] = { 0.0 };
    interface it = (interface) {
        4,
        1, sources,
        1, grounds,
        0, loads, weights
    };

    double vs[4] = { 5 };

    voltage_stimulation(ns, cc, it, vs);

    assert(fabs(ns.Vs[0] - 5.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[0]", 5.00, ns.Vs[0]); // a - source
    assert(fabs(ns.Vs[1] - 5.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[1]", 5.00, ns.Vs[1]); // b - disconnected from ground
    assert(fabs(ns.Vs[2] - 2.14) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[2]", 2.14, ns.Vs[2]); // c - voltage divider
    assert(fabs(ns.Vs[3] - 0.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[3]", 0.00, ns.Vs[3]); // d - ground
}

/**
 * Testing the following circuit:
 * 
 *                   SRC
 *                    |
 *      --------------------------------
 *     |              |                |
 *    R_1            R_2              R_3
 *     |              |                |
 *    R_4            R_5          ----------
 *     |              |          |         |
 *    --        -------------    |        R_6
 *             |     |      |   R_7        |
 *            R_8   R_9   R_10   |       R_11
 *             |     |      |    |         |
 *             -R_12--- R_13-    -----------
 *             |                      |
 *           R_14                    --
 *             |
 *            GND
 * 
 * All the resistances are 1kΩ.
 */
void test_complex()
{
    double nsYs[14];
    double nsVs[12];
    int ccIs[14] = {
        /* row 0 */ 1, 4, 8, /* row 1 */ 14, /* row 3 */ 44, 47, /* row 4 */ 57,
        /* row 5 */ 66, 67, 69, /* row 6 */ 81, 82, /* row 7 */ 93, /* row 8 */ 107
    };

    for (int i = 0; i < 14; i++)
    {
        nsYs[i] = 1;
    }

    network_state ns = { nsYs, nsVs };
    connected_component cc = { 12, 14, 0, 0, ccIs };

    bool sources[12] = { };
    sources[0] = true;
    bool grounds[12] = { };
    grounds[10] = true;
    bool loads[12] = { };
    double weights[12] = { 0.0 };
    interface it = (interface) {
        12,
        1, sources,
        1, grounds,
        0, loads, weights
    };

    double vs[12] = { 5 };

    voltage_stimulation(ns, cc, it, vs);

    assert(fabs(ns.Vs[0]  - 5.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[0]", 5.00, ns.Vs[0]);
    assert(fabs(ns.Vs[1]  - 5.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[1]", 5.00, ns.Vs[1]);
    assert(fabs(ns.Vs[2]  - 5.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[2]", 2.14, ns.Vs[2]);
    assert(fabs(ns.Vs[3]  - 5.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[3]", 0.00, ns.Vs[3]);
    assert(fabs(ns.Vs[4]  - 3.62) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[4]", 5.00, ns.Vs[4]);
    assert(fabs(ns.Vs[5]  - 1.90) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[5]", 5.00, ns.Vs[5]);
    assert(fabs(ns.Vs[6]  - 1.37) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[6]", 2.14, ns.Vs[6]);
    assert(fabs(ns.Vs[7]  - 2.07) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[7]", 0.00, ns.Vs[7]);
    assert(fabs(ns.Vs[8]  - 5.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[8]", 5.00, ns.Vs[8]);
    assert(fabs(ns.Vs[9]  - 2.25) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[9]", 5.00, ns.Vs[9]);
    assert(fabs(ns.Vs[10] - 0.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[10]", 2.14, ns.Vs[10]);
    assert(fabs(ns.Vs[11] - 5.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[11]", 0.00, ns.Vs[11]);
}

/**
 * Testing the following circuit:
 * 
 *              SRC
 *               |
 *        --R_1-----R_2---
 *       |               |
 *      --              R_L
 *                       |
 *                      GND
 * 
 * where:
 * - R_1 = 1.00 Ω
 * - R_2 = 0.25 Ω
 * - R_L = 0.50 Ω
 */
void test_load_one()
{
    double nsYs[2] = { 1, 4 };
    double nsVs[3];
    int ccIs[2] = { 1, 2 };

    network_state ns = { nsYs, nsVs };
    connected_component cc = { 3, 2, 0, 0, ccIs };

    bool sources[3] = { true, false, false };
    bool grounds[3] = { false, false, false };
    bool loads[3] = { false, false, true };
    double weights[3] = { .0, .0, 2.0 };
    interface it = (interface) {
        3,
        1, sources,
        0, grounds,
        1, loads, weights
    };

    double vs[5] = { 5 };

    voltage_stimulation(ns, cc, it, vs);

    assert(fabs(ns.Vs[0] - 5.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[0]", 5.00, ns.Vs[0]); // a - source
    assert(fabs(ns.Vs[1] - 5.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[1]", 5.00, ns.Vs[1]); // b - disconnected from ground
    assert(fabs(ns.Vs[2] - 3.33) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[2]", 3.33, ns.Vs[2]); // c - voltage divider
}

/**
 * Testing the following circuit:
 * 
 *      SRC2     SRC1
 *       |        |
 *        --R_1-----R_2---
 *       |               |
 *      --              R_L
 *                       |
 *                      GND
 * 
 * where:
 * - R_1 = 1.00 Ω
 * - R_2 = 0.25 Ω
 * - R_L = 0.50 Ω
 */
void test_load_two()
{
    double nsYs[2] = { 1, 4 };
    double nsVs[3];
    int ccIs[2] = { 1, 2 };

    network_state ns = { nsYs, nsVs };
    connected_component cc = { 3, 2, 0, 0, ccIs };

    bool sources[3] = { true, true, false };
    bool grounds[3] = { false, false, false };
    bool loads[3] = { false, false, true };
    double weights[3] = { .0, .0, 2.0 };
    interface it = (interface) {
        3,
        1, sources,
        0, grounds,
        1, loads, weights
    };

    double vs[5] = { 5, 10 };

    voltage_stimulation(ns, cc, it, vs);

    assert(fabs(ns.Vs[0] - 5.00)  < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[0]", 5.00,  ns.Vs[0]); // a - first source
    assert(fabs(ns.Vs[1] - 10.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[1]", 10.00, ns.Vs[1]); // b - second source
    assert(fabs(ns.Vs[2] - 3.33)  < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[2]", 3.33,  ns.Vs[2]); // c - voltage divider
}

/**
 * Testing the following circuit:
 * 
 *              SRC
 *               |
 *        --R_1-----R_2-----R_4---
 *       |              |        |
 *      --             R_3      R_L
 *                      |        |
 *                      ----------
 *                          |
 *                         GND
 * 
 * where:
 * - R_1 = 1.00 Ω
 * - R_2 = 0.40 Ω
 * - R_3 = 0.25 Ω
 * - R_4 = 1.00 Ω
 * - R_L = 0.50 Ω
 */
void test_grounded_and_loaded()
{
    // Ax = b -> A:
    // 
    //     ------------------------------
    //     |       b + 2.5c         | a |
    //     |  a                     | b |
    //     | 2.5a         + 1d + 4e | c |
    //     |           1c      + 2e | d |
    //     |           4c + 2d      | e |
    //     ------------------------------
    double nsYs[4] = { 1, 2.5, 1, 4 };
    double nsVs[5];
    int ccIs[4] = { 1, 2, 13, 14 };

    network_state ns = { nsYs, nsVs };
    connected_component cc = { 5, 4, 0, 0, ccIs };

    bool sources[5] = { true, false, false, false, false };
    bool grounds[5] = { false, false, false, false, true };
    bool loads[5] = { false, false, false, true, false };
    double weights[5] = { .0, .0, .0, 2.0, .0 };
    interface it = (interface) {
        5,
        1, sources,
        1, grounds,
        1, loads, weights
    };

    double vs[5] = { 5 };

    voltage_stimulation(ns, cc, it, vs);

    assert(fabs(ns.Vs[0] - 5.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[0]", 5.00, ns.Vs[0]); // a - source
    assert(fabs(ns.Vs[1] - 5.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[1]", 5.00, ns.Vs[1]); // b - disconnected from ground
    assert(fabs(ns.Vs[2] - 1.72) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[2]", 1.72, ns.Vs[2]); // c - voltage divider
    assert(fabs(ns.Vs[3] - 0.57) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[3]", 0.57, ns.Vs[3]); // d - voltage divider
    assert(fabs(ns.Vs[4] - 0.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[4]", 0.00, ns.Vs[4]); // e - ground
}

/**
 * Testing the following circuit:
 * 
 *      5 V          10 V
 *       |            |
 *       ---R------R---
 *             |
 *             R
 *             |
 *            GND
 */
void test_input_currents()
{
    double nsYs[3] = { 1, 0.33, 1 };
    double nsVs[4];
    int ccIs[3] = { 1, 6, 7 };

    network_state ns = { nsYs, nsVs };
    connected_component cc = { 4, 3, 0, 0, ccIs };

    bool sources[4] = { true, false, true, false };
    bool grounds[4] = { false, false, false, true };
    bool loads[4] = { false, false, false, false };
    interface it = (interface) {
        4,
        2, sources,
        1, grounds,
        0, loads, NULL
    };

    double vs[4] = { 5, 0, 10 };

    voltage_stimulation(ns, cc, it, vs);

    assert(fabs(ns.Vs[0] - 5.00)  < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[0]",  5.00, ns.Vs[0]); // a - source 5 v
    assert(fabs(ns.Vs[1] - 3.57)  < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[1]",  3.57, ns.Vs[1]); // b - voltage divider
    assert(fabs(ns.Vs[2] - 10.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[2]", 10.00, ns.Vs[2]); // c - source 10 v
    assert(fabs(ns.Vs[3] - 0.00)  < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[3]",  0.00, ns.Vs[3]); // d - ground

    assert(fabs(vs[0] - 1.43) < TOLERANCE, -1, "I_1 == %f but should be %f", vs[0], 1.43);  // in mA
    assert(fabs(vs[2] - 2.14) < TOLERANCE, -1, "I_2 == %f but should be %f", vs[1], 2.14);  // in mA
}

/**
 * Testing the following circuit:
 * 
 *      SRC-----R-----R-----GND
 * 
 *      SRC-----R-----R-----R-----L-----GND
 */
void test_multiple_connected_components()
{
    double nsYs[5] = { 1, 1, 1, 1, 1 };
    double nsVs[7];
    // ns.Is = /* CC 1 */ 2, 18, /* CC 2 */  12, 26, 27

    int cc1Is[2] = { 1, 5 };
    int cc2Is[3] = { 2, 6, 7 };

    network_state ns = { nsYs, nsVs };
    connected_component cc1 = { 3, 2, 0, 0, cc1Is };
    connected_component cc2 = { 4, 3, 3, 2, cc2Is };

    bool sources[7] = { true, false, false, true, false, false, false };
    bool grounds[7] = { false, false, true, false, false, false, false };
    bool loads[7] = { false, false, false, false, false, false, true };
    double weights[7] = { 0, 0, 0, 0, 0, 0, 2 }; // TODO 1 / R
    interface it = (interface) {
        7,
        2, sources,
        1, grounds,
        1, loads, weights
    };

    double vs[7] = { 5, 0, 0, 5 };

    voltage_stimulation(ns, cc1, it, vs);

    assert(fabs(ns.Vs[0] - 5.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[0]", 5.00, ns.Vs[0]); // a - source
    assert(fabs(ns.Vs[1] - 2.50) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[1]", 2.50, ns.Vs[1]); // b - voltage divider
    assert(fabs(ns.Vs[2] - 0.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[2]", 0.00, ns.Vs[2]); // c - ground

    voltage_stimulation(ns, cc2, it, vs);

    assert(fabs(ns.Vs[3] - 5.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[3]", 5.00, ns.Vs[3]); // a - source
    assert(fabs(ns.Vs[4] - 2.14) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[4]", 2.14, ns.Vs[4]); // b - voltage divider
    assert(fabs(ns.Vs[5] - 3.57) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[5]", 3.57, ns.Vs[5]); // b - voltage divider
    assert(fabs(ns.Vs[6] - 0.71) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[6]", 0.71, ns.Vs[6]); // c - ground

    // check that the voltage stimulation affects only the nodes of a CC
    assert(fabs(ns.Vs[0] - 5.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[0]", 5.00, ns.Vs[0]); // a - source
    assert(fabs(ns.Vs[1] - 2.50) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[1]", 2.50, ns.Vs[1]); // b - voltage divider
    assert(fabs(ns.Vs[2] - 0.00) < TOLERANCE, -1, DOUBLE_ERROR, "ns.Vs[2]", 0.00, ns.Vs[2]); // c - ground
}

int stimulator_mna()
{
    test_divider_one();
    test_divider_two();
    test_parallel();
    test_complex();
    test_load_one();
    test_load_two();
    test_grounded_and_loaded();
    test_input_currents();
    test_multiple_connected_components();

    return 0;
}
