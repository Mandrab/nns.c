#include "test_mna.h"
#include "device/network.h"
#include "stimulator/mna.h"
#include "util/tensors.h"

/**
 * Testing the following circuit:
 * 
 *      SRC-----R-----R-----GND
 */
START_TEST(test_modified_nodal_analysis_minimal)
{
    const int size = 3;

    network_state ns = (network_state) {
        size,
        matrix(bool, size, size),
        matrix(float, size, size),
        matrix(float, size, size),
        vector(float, size)
    };

    ns.Y[0][0] = 0; ns.Y[0][1] = 1; ns.Y[0][2] = 0;
    ns.Y[1][0] = 1; ns.Y[1][1] = 0; ns.Y[1][2] = 1;
    ns.Y[2][0] = 0; ns.Y[2][1] = 1; ns.Y[2][2] = 0;

    bool sources[3] = { true, false, false };
    bool grounds[3] = { false, false, true };
    bool loads[3] = { false, false, false };
    float weights[3] = { 0.0 };
    interface it = (interface) {
        1, sources,
        1, grounds,
        0, loads, weights
    };

    // define the voltage of the first input
    float vs[1] = { 5 };

    // call the conjugate_gradient function
    voltage_stimulation(ns, it, vs);

    ck_assert_float_eq_tol(ns.V[0],  5.00, 1e-2); // a - source
    ck_assert_float_eq_tol(ns.V[1],  2.50, 1e-2); // b - voltage divider
    ck_assert_float_eq_tol(ns.V[2],  0.00, 1e-2); // c - ground

    free_matrix(ns.A, size);
    free_matrix(ns.G, size);
    free_matrix(ns.Y, size);
    free(ns.V);
}
END_TEST

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
START_TEST(test_modified_nodal_analysis_basic)
{
    const int size = 4;

    network_state ns = (network_state) {
        size,
        matrix(bool, size, size),
        matrix(float, size, size),
        matrix(float, size, size),
        vector(float, size)
    };

    // Ax = b:
    //
    //     -------------------------
    //     |       b + 3c      | a | = 0
    //     |  a                | b | = 1
    //     | 3a           + 4d | c | = 2
    //     |           4c      | d | = 1
    //     -------------------------
    ns.Y[0][0] = 0; ns.Y[0][1] = 1; ns.Y[0][2] = 3; ns.Y[0][3] = 0;
    ns.Y[1][0] = 1; ns.Y[1][1] = 0; ns.Y[1][2] = 0; ns.Y[1][3] = 0;
    ns.Y[2][0] = 3; ns.Y[2][1] = 0; ns.Y[2][2] = 0; ns.Y[2][3] = 4;
    ns.Y[3][0] = 0; ns.Y[3][1] = 0; ns.Y[3][2] = 4; ns.Y[3][3] = 0;

    bool sources[4] = { true, false, false, false };
    bool grounds[4] = { false, false, false, true };
    bool loads[4] = { false, false, false, false };
    float weights[4] = { 0.0 };
    interface it = (interface) {
        1, sources,
        1, grounds,
        0, loads, weights
    };

    // define the voltage of the first input
    float vs[1] = { 5 };

    // call the conjugate_gradient function
    voltage_stimulation(ns, it, vs);

    ck_assert_float_eq_tol(ns.V[0],  5.00, 1e-2); // a - source
    ck_assert_float_eq_tol(ns.V[1],  5.00, 1e-2); // b - disconnected from ground
    ck_assert_float_eq_tol(ns.V[2],  2.14, 1e-2); // c - voltage divider
    ck_assert_float_eq_tol(ns.V[3],  0.00, 1e-2); // d - ground

    free_matrix(ns.A, size);
    free_matrix(ns.G, size);
    free_matrix(ns.Y, size);
    free(ns.V);
}
END_TEST

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
START_TEST(test_modified_nodal_analysis_advanced)
{
    const int size = 12;

    network_state ns = (network_state) {
        size,
        matrix(bool, size, size),
        matrix(float, size, size),
        matrix(float, size, size),
        vector(float, size)
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
    float weights[12] = { 0.0 };
    interface it = (interface) {
        1, sources,
        1, grounds,
        0, loads, weights
    };

    // define the voltage of the first input
    float vs[1] = { 5 };

    // call the conjugate_gradient function
    voltage_stimulation(ns, it, vs);

    ck_assert_float_eq_tol(ns.V[0],  5.00, 1e-2);
    ck_assert_float_eq_tol(ns.V[1],  5.00, 1e-2);
    ck_assert_float_eq_tol(ns.V[2],  5.00, 1e-2);
    ck_assert_float_eq_tol(ns.V[3],  5.00, 1e-2);
    ck_assert_float_eq_tol(ns.V[4],  3.62, 1e-2);
    ck_assert_float_eq_tol(ns.V[5],  1.90, 1e-2);
    ck_assert_float_eq_tol(ns.V[6],  1.37, 1e-2);
    ck_assert_float_eq_tol(ns.V[7],  2.07, 1e-2);
    ck_assert_float_eq_tol(ns.V[8],  5.00, 1e-2);
    ck_assert_float_eq_tol(ns.V[9],  2.25, 1e-2);
    ck_assert_float_eq_tol(ns.V[10], 0.00, 1e-2);
    ck_assert_float_eq_tol(ns.V[11], 5.00, 1e-2);

    free_matrix(ns.A, size);
    free_matrix(ns.G, size);
    free_matrix(ns.Y, size);
    free(ns.V);
}
END_TEST

Suite* mna_suite()
{
    Suite* suite = suite_create("Modified nodal analysis");
    TCase* tc_core = tcase_create("Distribution of the voltages in a circuit connected with a source and a ground");

    tcase_add_test(tc_core, test_modified_nodal_analysis_minimal);
    tcase_add_test(tc_core, test_modified_nodal_analysis_basic);
    tcase_add_test(tc_core, test_modified_nodal_analysis_advanced);
    suite_add_tcase(suite, tc_core);

    return suite;
}
