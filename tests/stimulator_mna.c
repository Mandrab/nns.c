#include <math.h>

#include "device/network.h"
#include "stimulator/mna.h"
#include "util/errors.h"
#include "util/tensors.h"

// allows 0.05 volt difference between calculated and expected
#define TOLERANCE 5e-2

/**
 * Testing the following circuit:
 * 
 *      SRC-----R-----R-----GND
 */
void test_modified_nodal_analysis_divider()
{
    const int size = 3;

    network_state ns = (network_state) {
        size,
        matrix(bool, size, size),
        matrix(double, size, size),
        matrix(double, size, size),
        vector(double, size)
    };

    ns.Y[0][0] = 0; ns.Y[0][1] = 1; ns.Y[0][2] = 0;
    ns.Y[1][0] = 1; ns.Y[1][1] = 0; ns.Y[1][2] = 1;
    ns.Y[2][0] = 0; ns.Y[2][1] = 1; ns.Y[2][2] = 0;

    bool sources[3] = { true, false, false };
    bool grounds[3] = { false, false, true };
    bool loads[3] = { false, false, false };
    double weights[3] = { 0.0 };
    interface it = (interface) {
        1, sources,
        1, grounds,
        0, loads, weights
    };

    // define the voltage of the first input
    double vs[1] = { 5 };

    // call the conjugate_gradient function
    voltage_stimulation(ns, it, vs);

    assert(fabs(ns.V[0] - 5.00) < TOLERANCE, -1, "ns.V[0] == %f but should be %f", ns.V[0], 5.00); // a - source
    assert(fabs(ns.V[1] - 2.50) < TOLERANCE, -1, "ns.V[1] == %f but should be %f", ns.V[1], 2.50); // b - voltage divider
    assert(fabs(ns.V[2] - 0.00) < TOLERANCE, -1, "ns.V[2] == %f but should be %f", ns.V[2], 0.00); // c - ground

    free_matrix(ns.A, size);
    free_matrix(ns.G, size);
    free_matrix(ns.Y, size);
    free(ns.V);
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
void test_modified_nodal_analysis_parallel()
{
    const int size = 4;

    network_state ns = (network_state) {
        size,
        matrix(bool, size, size),
        matrix(double, size, size),
        matrix(double, size, size),
        vector(double, size)
    };

    // Ax = b -> A:
    //
    //     -------------------------
    //     |       b + 3c      | a |
    //     |  a                | b |
    //     | 3a           + 4d | c |
    //     |           4c      | d |
    //     -------------------------
    ns.Y[0][0] = 0; ns.Y[0][1] = 1; ns.Y[0][2] = 3; ns.Y[0][3] = 0;
    ns.Y[1][0] = 1; ns.Y[1][1] = 0; ns.Y[1][2] = 0; ns.Y[1][3] = 0;
    ns.Y[2][0] = 3; ns.Y[2][1] = 0; ns.Y[2][2] = 0; ns.Y[2][3] = 4;
    ns.Y[3][0] = 0; ns.Y[3][1] = 0; ns.Y[3][2] = 4; ns.Y[3][3] = 0;

    bool sources[4] = { true, false, false, false };
    bool grounds[4] = { false, false, false, true };
    bool loads[4] = { false, false, false, false };
    double weights[4] = { 0.0 };
    interface it = (interface) {
        1, sources,
        1, grounds,
        0, loads, weights
    };

    // define the voltage of the first input
    double vs[1] = { 5 };

    // call the conjugate_gradient function
    voltage_stimulation(ns, it, vs);

    assert(fabs(ns.V[0] - 5.00) < TOLERANCE, -1, "ns.V[0] == %f but should be %f", ns.V[0], 5.00); // a - source
    assert(fabs(ns.V[1] - 5.00) < TOLERANCE, -1, "ns.V[1] == %f but should be %f", ns.V[1], 5.00); // b - disconnected from ground
    assert(fabs(ns.V[2] - 2.14) < TOLERANCE, -1, "ns.V[2] == %f but should be %f", ns.V[2], 2.14); // c - voltage divider
    assert(fabs(ns.V[3] - 0.00) < TOLERANCE, -1, "ns.V[3] == %f but should be %f", ns.V[3], 0.00); // d - ground

    free_matrix(ns.A, size);
    free_matrix(ns.G, size);
    free_matrix(ns.Y, size);
    free(ns.V);
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
void test_modified_nodal_analysis_complex()
{
    const int size = 12;

    network_state ns = (network_state) {
        size,
        matrix(bool, size, size),
        matrix(double, size, size),
        matrix(double, size, size),
        vector(double, size)
    };

    ns.Y[0][0] = 0; ns.Y[0][1] = 1; ns.Y[0][2] = 0; ns.Y[0][3] = 0; ns.Y[0][4] = 1; ns.Y[0][5] = 0;
    ns.Y[0][6] = 0; ns.Y[0][7] = 0; ns.Y[0][8] = 1; ns.Y[0][9] = 0; ns.Y[0][10] = 0; ns.Y[0][11] = 0;

    ns.Y[1][0] = 1; ns.Y[1][1] = 0; ns.Y[1][2] = 1; ns.Y[1][3] = 0; ns.Y[1][4] = 0; ns.Y[1][5] = 0;
    ns.Y[1][6] = 0; ns.Y[1][7] = 0; ns.Y[1][8] = 0; ns.Y[1][9] = 0; ns.Y[1][10] = 0; ns.Y[1][11] = 0;

    ns.Y[2][0] = 0; ns.Y[2][1] = 1; ns.Y[2][2] = 0; ns.Y[2][3] = 0; ns.Y[2][4] = 0; ns.Y[2][5] = 0;
    ns.Y[2][6] = 0; ns.Y[2][7] = 0; ns.Y[2][8] = 0; ns.Y[2][9] = 0; ns.Y[2][10] = 0; ns.Y[2][11] = 0;

    ns.Y[3][0] = 0; ns.Y[3][1] = 0; ns.Y[3][2] = 0; ns.Y[3][3] = 0; ns.Y[3][4] = 0; ns.Y[3][5] = 0;
    ns.Y[3][6] = 0; ns.Y[3][7] = 0; ns.Y[3][8] = 1; ns.Y[3][9] = 0; ns.Y[3][10] = 0; ns.Y[3][11] = 1;

    ns.Y[4][0] = 1; ns.Y[4][1] = 0; ns.Y[4][2] = 0; ns.Y[4][3] = 0; ns.Y[4][4] = 0; ns.Y[4][5] = 0;
    ns.Y[4][6] = 0; ns.Y[4][7] = 0; ns.Y[4][8] = 0; ns.Y[4][9] = 1; ns.Y[4][10] = 0; ns.Y[4][11] = 0;

    ns.Y[5][0] = 0; ns.Y[5][1] = 0; ns.Y[5][2] = 0; ns.Y[5][3] = 0; ns.Y[5][4] = 0; ns.Y[5][5] = 0;
    ns.Y[5][6] = 1; ns.Y[5][7] = 1; ns.Y[5][8] = 0; ns.Y[5][9] = 1; ns.Y[5][10] = 0; ns.Y[5][11] = 0;

    ns.Y[6][0] = 0; ns.Y[6][1] = 0; ns.Y[6][2] = 0; ns.Y[6][3] = 0; ns.Y[6][4] = 0; ns.Y[6][5] = 1;
    ns.Y[6][6] = 0; ns.Y[6][7] = 0; ns.Y[6][8] = 0; ns.Y[6][9] = 1; ns.Y[6][10] = 1; ns.Y[6][11] = 0;

    ns.Y[7][0] = 0; ns.Y[7][1] = 0; ns.Y[7][2] = 0; ns.Y[7][3] = 0; ns.Y[7][4] = 0; ns.Y[7][5] = 1;
    ns.Y[7][6] = 0; ns.Y[7][7] = 0; ns.Y[7][8] = 0; ns.Y[7][9] = 1; ns.Y[7][10] = 0; ns.Y[7][11] = 0;

    ns.Y[8][0] = 1; ns.Y[8][1] = 0; ns.Y[8][2] = 0; ns.Y[8][3] = 1; ns.Y[8][4] = 0; ns.Y[8][5] = 0;
    ns.Y[8][6] = 0; ns.Y[8][7] = 0; ns.Y[8][8] = 0; ns.Y[8][9] = 0; ns.Y[8][10] = 0; ns.Y[8][11] = 1;

    ns.Y[9][0] = 0; ns.Y[9][1] = 0; ns.Y[9][2] = 0; ns.Y[9][3] = 0; ns.Y[9][4] = 1; ns.Y[9][5] = 1;
    ns.Y[9][6] = 1; ns.Y[9][7] = 1; ns.Y[9][8] = 0; ns.Y[9][9] = 0; ns.Y[9][10] = 0; ns.Y[9][11] = 0;

    ns.Y[10][0] = 0; ns.Y[10][1] = 0; ns.Y[10][2] = 0; ns.Y[10][3] = 0; ns.Y[10][4] = 0; ns.Y[10][5] = 0;
    ns.Y[10][6] = 1; ns.Y[10][7] = 0; ns.Y[10][8] = 0; ns.Y[10][9] = 0; ns.Y[10][10] = 0; ns.Y[10][11] = 0;

    ns.Y[11][0] = 0; ns.Y[11][1] = 0; ns.Y[11][2] = 0; ns.Y[11][3] = 1; ns.Y[11][4] = 0; ns.Y[11][5] = 0;
    ns.Y[11][6] = 0; ns.Y[11][7] = 0; ns.Y[11][8] = 1; ns.Y[11][9] = 0; ns.Y[11][10] = 0; ns.Y[11][11] = 0;


    bool sources[12] = { };
    sources[0] = true;
    bool grounds[12] = { };
    grounds[10] = true;
    bool loads[12] = { };
    double weights[12] = { 0.0 };
    interface it = (interface) {
        1, sources,
        1, grounds,
        0, loads, weights
    };

    // define the voltage of the first input
    double vs[1] = { 5 };

    // call the conjugate_gradient function
    voltage_stimulation(ns, it, vs);

    assert(fabs(ns.V[0]  - 5.00) < TOLERANCE, -1, "ns.V[0] == %f but should be %f", ns.V[0], 5.00);
    assert(fabs(ns.V[1]  - 5.00) < TOLERANCE, -1, "ns.V[1] == %f but should be %f", ns.V[1], 5.00);
    assert(fabs(ns.V[2]  - 5.00) < TOLERANCE, -1, "ns.V[2] == %f but should be %f", ns.V[2], 2.14);
    assert(fabs(ns.V[3]  - 5.00) < TOLERANCE, -1, "ns.V[3] == %f but should be %f", ns.V[3], 0.00);
    assert(fabs(ns.V[4]  - 3.62) < TOLERANCE, -1, "ns.V[4] == %f but should be %f", ns.V[4], 5.00);
    assert(fabs(ns.V[5]  - 1.90) < TOLERANCE, -1, "ns.V[5] == %f but should be %f", ns.V[5], 5.00);
    assert(fabs(ns.V[6]  - 1.37) < TOLERANCE, -1, "ns.V[6] == %f but should be %f", ns.V[6], 2.14);
    assert(fabs(ns.V[7]  - 2.07) < TOLERANCE, -1, "ns.V[7] == %f but should be %f", ns.V[7], 0.00);
    assert(fabs(ns.V[8]  - 5.00) < TOLERANCE, -1, "ns.V[8] == %f but should be %f", ns.V[8], 5.00);
    assert(fabs(ns.V[9]  - 2.25) < TOLERANCE, -1, "ns.V[9] == %f but should be %f", ns.V[9], 5.00);
    assert(fabs(ns.V[10] - 0.00) < TOLERANCE, -1, "ns.V[10] == %f but should be %f", ns.V[10], 2.14);
    assert(fabs(ns.V[11] - 5.00) < TOLERANCE, -1, "ns.V[11] == %f but should be %f", ns.V[11], 0.00);

    free_matrix(ns.A, size);
    free_matrix(ns.G, size);
    free_matrix(ns.Y, size);
    free(ns.V);
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
void test_modified_nodal_analysis_loaded()
{
    const int size = 3;

    network_state ns = (network_state) {
        size,
        matrix(bool, size, size),
        matrix(double, size, size),
        matrix(double, size, size),
        vector(double, size)
    };

    ns.Y[0][0] = 0; ns.Y[0][1] = 1; ns.Y[0][2] = 4;
    ns.Y[1][0] = 1; ns.Y[1][1] = 0; ns.Y[1][2] = 0;
    ns.Y[2][0] = 4; ns.Y[2][1] = 0; ns.Y[2][2] = 0;

    bool sources[5] = { true, false, false };
    bool grounds[5] = { false, false, false };
    bool loads[5] = { false, false, true };
    double weights[5] = { .0, .0, 2.0 };
    interface it = (interface) {
        1, sources,
        0, grounds,
        1, loads, weights
    };

    // define the voltage of the first input
    double vs[1] = { 5 };

    // call the conjugate_gradient function
    voltage_stimulation(ns, it, vs);

    assert(fabs(ns.V[0] - 5.00) < TOLERANCE, -1, "ns.V[0] == %f but should be %f", ns.V[0], 5.00); // a - source
    assert(fabs(ns.V[1] - 5.00) < TOLERANCE, -1, "ns.V[1] == %f but should be %f", ns.V[1], 5.00); // b - disconnected from ground
    assert(fabs(ns.V[2] - 3.33) < TOLERANCE, -1, "ns.V[2] == %f but should be %f", ns.V[2], 3.33); // c - voltage divider

    free_matrix(ns.A, size);
    free_matrix(ns.G, size);
    free_matrix(ns.Y, size);
    free(ns.V);
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
void test_modified_nodal_analysis_grounded_and_loaded()
{
    const int size = 5;

    network_state ns = (network_state) {
        size,
        matrix(bool, size, size),
        matrix(double, size, size),
        matrix(double, size, size),
        vector(double, size)
    };

    // Ax = b -> A:
    // 
    //     ------------------------------
    //     |       b + 2.5c         | a |
    //     |  a                     | b |
    //     | 2.5a         + 1d + 4e | c |
    //     |           1c      + 2e | d |
    //     |           4c + 2d      | e |
    //     ------------------------------
    ns.Y[0][0] = 0.0; ns.Y[0][1] = 1.0; ns.Y[0][2] = 2.5; ns.Y[0][3] = 0.0; ns.Y[0][4] = 0.0;
    ns.Y[1][0] = 1.0; ns.Y[1][1] = 0.0; ns.Y[1][2] = 0.0; ns.Y[1][3] = 0.0; ns.Y[1][4] = 0.0;
    ns.Y[2][0] = 2.5; ns.Y[2][1] = 0.0; ns.Y[2][2] = 0.0; ns.Y[2][3] = 1.0; ns.Y[2][4] = 4.0;
    ns.Y[3][0] = 0.0; ns.Y[3][1] = 0.0; ns.Y[3][2] = 1.0; ns.Y[3][3] = 0.0; ns.Y[3][4] = 0.0;
    ns.Y[4][0] = 0.0; ns.Y[4][1] = 0.0; ns.Y[4][2] = 4.0; ns.Y[4][3] = 0.0; ns.Y[4][4] = 0.0;

    bool sources[5] = { true, false, false, false, false };
    bool grounds[5] = { false, false, false, false, true };
    bool loads[5] = { false, false, false, true, false };
    double weights[5] = { .0, .0, .0, 2.0, .0 };
    interface it = (interface) {
        1, sources,
        1, grounds,
        1, loads, weights
    };

    // define the voltage of the first input
    double vs[1] = { 5 };

    // call the conjugate_gradient function
    voltage_stimulation(ns, it, vs);

    assert(fabs(ns.V[0] - 5.00) < TOLERANCE, -1, "ns.V[0] == %f but should be %f", ns.V[0], 5.00); // a - source
    assert(fabs(ns.V[1] - 5.00) < TOLERANCE, -1, "ns.V[1] == %f but should be %f", ns.V[1], 5.00); // b - disconnected from ground
    assert(fabs(ns.V[2] - 1.72) < TOLERANCE, -1, "ns.V[2] == %f but should be %f", ns.V[2], 1.72); // c - voltage divider
    assert(fabs(ns.V[3] - 0.57) < TOLERANCE, -1, "ns.V[3] == %f but should be %f", ns.V[3], 0.57); // d - voltage divider
    assert(fabs(ns.V[4] - 0.00) < TOLERANCE, -1, "ns.V[4] == %f but should be %f", ns.V[4], 0.00); // e - ground

    free_matrix(ns.A, size);
    free_matrix(ns.G, size);
    free_matrix(ns.Y, size);
    free(ns.V);
}

int stimulator_mna()
{
    test_modified_nodal_analysis_divider();
    test_modified_nodal_analysis_parallel();
    test_modified_nodal_analysis_complex();
    test_modified_nodal_analysis_loaded();
    test_modified_nodal_analysis_grounded_and_loaded();

    return 0;
}
