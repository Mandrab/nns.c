#include <check.h>
#include <float.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "test_components.h"
#include "util/components.h"
#include "util/tensors.h"

#define SAMPLES_COUNT  1000000
#define QUANTS_COUNT 100
#define ACCURACY 0.001      // 0.1 % accuracy

START_TEST(test_fully_connected_network)
{
    bool** A = matrix(bool, 3, 3);
    A[0][0] = 0; A[0][1] = 1; A[0][2] = 1;
    A[1][0] = 1; A[1][1] = 0; A[1][2] = 0;
    A[2][0] = 1; A[2][1] = 0; A[2][2] = 0;

    float** G = matrix(float, 3, 3);
    G[0][0] = 0; G[0][1] = 1; G[0][2] = 2;
    G[1][0] = 1; G[1][1] = 3; G[1][2] = 4;
    G[2][0] = 2; G[2][1] = 4; G[2][2] = 5;

    float** Y = matrix(float, 3, 3);
    Y[0][0] = 10; Y[0][1] = 11; Y[0][2] = 12;
    Y[1][0] = 11; Y[1][1] = 13; Y[1][2] = 14;
    Y[2][0] = 12; Y[2][1] = 14; Y[2][2] = 15;

    float* V = vector(float, 3);
    V[0] = 16; V[1] = 17;  V[2] = 18;

    network_state ns = (network_state) { 3, A, G, Y, V };

    int* nodes_in_cc, nss_count;

    network_state* nss = connected_components(ns, &nss_count, &nodes_in_cc);

    ck_assert_msg(nss_count == 1, "Wrong number of connected components identified");
    ck_assert_msg(
        nodes_in_cc[0] == nss[0].size && nss[0].size == 3,
        "The # of nodes in the CC (%d) is different from the expected 3 and the discovered (%d)",
        nodes_in_cc[0], nss[0].size
    );

    network_state* lns = largest_component(nss, nss_count, nodes_in_cc);

    ck_assert_msg(lns == &(nss[0]), "The largest connected component does not point correctly");
}
END_TEST

START_TEST(test_disjoint_network){
    bool** A = matrix(bool, 6, 6);
    A[0][0] = 0; A[0][1] = 1; A[0][2] = 0; A[0][3] = 1; A[0][4] = 0; A[0][5] = 0;
    A[1][0] = 1; A[1][1] = 0; A[1][2] = 0; A[1][3] = 0; A[1][4] = 0; A[1][5] = 0;
    A[2][0] = 0; A[2][1] = 0; A[2][2] = 0; A[2][3] = 0; A[2][4] = 0; A[2][5] = 0;
    A[3][0] = 1; A[3][1] = 0; A[3][2] = 0; A[3][3] = 0; A[3][4] = 0; A[3][5] = 0;
    A[4][0] = 0; A[4][1] = 0; A[4][2] = 0; A[4][3] = 0; A[4][4] = 0; A[4][5] = 1;
    A[5][0] = 0; A[5][1] = 0; A[5][2] = 0; A[5][3] = 0; A[5][4] = 1; A[5][5] = 0;

    float** G = matrix(float, 6, 6);
    G[0][0] = 0; G[0][1] = 1;  G[0][2] = 2;  G[0][3] = 3;  G[0][4] = 4;  G[0][5] = 5;
    G[1][0] = 1; G[1][1] = 6;  G[1][2] = 7;  G[1][3] = 8;  G[1][4] = 9;  G[1][5] = 10;
    G[2][0] = 2; G[2][1] = 7;  G[2][2] = 11; G[2][3] = 12; G[2][4] = 13; G[2][5] = 14;
    G[3][0] = 3; G[3][1] = 8;  G[3][2] = 12; G[3][3] = 15; G[3][4] = 16; G[3][5] = 17;
    G[4][0] = 4; G[4][1] = 9;  G[4][2] = 13; G[4][3] = 16; G[4][4] = 18; G[4][5] = 19;
    G[5][0] = 5; G[5][1] = 10; G[5][2] = 14; G[5][3] = 17; G[5][4] = 19; G[5][5] = 20;

    float** Y = matrix(float, 6, 6);
    Y[0][0] = 21; Y[0][1] = 22; Y[0][2] = 23; Y[0][3] = 24; Y[0][4] = 25; Y[0][5] = 26;
    Y[1][0] = 22; Y[1][1] = 27; Y[1][2] = 28; Y[1][3] = 29; Y[1][4] = 30; Y[1][5] = 31;
    Y[2][0] = 23; Y[2][1] = 28; Y[2][2] = 32; Y[2][3] = 33; Y[2][4] = 34; Y[2][5] = 35;
    Y[3][0] = 24; Y[3][1] = 29; Y[3][2] = 33; Y[3][3] = 36; Y[3][4] = 37; Y[3][5] = 38;
    Y[4][0] = 25; Y[4][1] = 30; Y[4][2] = 34; Y[4][3] = 37; Y[4][4] = 39; Y[4][5] = 40;
    Y[5][0] = 26; Y[5][1] = 31; Y[5][2] = 35; Y[5][3] = 38; Y[5][4] = 40; Y[5][5] = 41;

    float* V = vector(float, 6);
    V[0] = 42; V[1] = 43;  V[2] = 44; V[3] = 45; V[4] = 46;  V[5] = 47;

    network_state ns = (network_state) { 6, A, G, Y, V };

    int* nodes_in_cc, nss_count;

    network_state* nss = connected_components(ns, &nss_count, &nodes_in_cc);

    ck_assert_msg(nss_count == 3, "Wrong number of connected components identified");

    char* error = "The # of nodes in the CC (%d) is different from the expected 3 and the discovered (%d)";
    ck_assert_msg(nodes_in_cc[0] == nss[0].size && nss[0].size == 3, error, nodes_in_cc[0], nss[0].size);
    ck_assert_msg(nodes_in_cc[1] == nss[1].size && nss[1].size == 1, error, nodes_in_cc[1], nss[1].size);
    ck_assert_msg(nodes_in_cc[2] == nss[2].size && nss[2].size == 2, error, nodes_in_cc[2], nss[2].size);

    error = "Error in the matrix sub-division for the first CC";

    ck_assert_msg(nss[0].A[0][0] == 0 && nss[0].G[0][0] == 0 && nss[0].Y[0][0] == 21, error);
    ck_assert_msg(nss[0].A[0][1] == 1 && nss[0].G[0][1] == 1 && nss[0].Y[0][1] == 22, error);
    ck_assert_msg(nss[0].A[0][2] == 1 && nss[0].G[0][2] == 3 && nss[0].Y[0][2] == 24, error);
    ck_assert_msg(
        nss[0].A[0][1] == nss[0].A[1][0] &&
        nss[0].G[0][1] == nss[0].G[1][0] &&
        nss[0].Y[0][1] == nss[0].Y[1][0],
        error
    );
    ck_assert_msg(
        nss[0].A[0][2] == nss[0].A[2][0] &&
        nss[0].G[0][2] == nss[0].G[2][0] &&
        nss[0].Y[0][2] == nss[0].Y[2][0],
        error
    );
    ck_assert_msg(nss[0].A[1][1] == 0 && nss[0].G[1][1] == 6 && nss[0].Y[1][1] == 27, error);
    ck_assert_msg(nss[0].A[1][2] == 0 && nss[0].G[1][2] == 8 && nss[0].Y[1][2] == 29, error);
    ck_assert_msg(
        nss[0].A[1][2] == nss[0].A[2][1] &&
        nss[0].G[1][2] == nss[0].G[2][1] &&
        nss[0].Y[1][2] == nss[0].Y[2][1],
        error
    );
    ck_assert_msg(nss[0].A[2][2] == 0 && nss[0].G[2][2] == 15 && nss[0].Y[2][2] == 36, error);

    error = "Error in the matrix sub-division for the second CC";
    
    ck_assert_msg(nss[1].A[0][0] == 0 && nss[1].G[0][0] == 11 && nss[1].Y[0][0] == 32, error);

    error = "Error in the matrix sub-division for the third CC";

    ck_assert_msg(nss[2].A[0][0] == 0 && nss[2].G[0][0] == 18 && nss[2].Y[0][0] == 39, error);
    ck_assert_msg(nss[2].A[0][1] == 1 && nss[2].G[0][1] == 19 && nss[2].Y[0][1] == 40, error);
    ck_assert_msg(nss[2].A[1][0] == 1 && nss[2].G[1][0] == 19 && nss[2].Y[1][0] == 40, error);
    ck_assert_msg(nss[2].A[1][1] == 0 && nss[2].G[1][1] == 20 && nss[2].Y[1][1] == 41, error);

    network_state* lns = largest_component(nss, nss_count, nodes_in_cc);

    ck_assert_msg(lns == &(nss[0]), "The largest connected component does not point correctly");
}
END_TEST

Suite* components_suite()
{
    Suite* suite = suite_create("Components");
    TCase* tc_core = tcase_create("Connected components discovery");

    tcase_add_test(tc_core, test_fully_connected_network);
    tcase_add_test(tc_core, test_disjoint_network);
    suite_add_tcase(suite, tc_core);

    return suite;
}
