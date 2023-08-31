#include <float.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "util/components.h"
#include "util/errors.h"
#include "util/tensors.h"

#define SAMPLES_COUNT  1000000
#define QUANTS_COUNT 100
#define ACCURACY 0.001      // 0.1 % accuracy

void test_fully_connected_network()
{
    bool** A = matrix(bool, 3, 3);
    A[0][0] = 0; A[0][1] = 1; A[0][2] = 1;
    A[1][0] = 1; A[1][1] = 0; A[1][2] = 0;
    A[2][0] = 1; A[2][1] = 0; A[2][2] = 0;

    double** Y = matrix(double, 3, 3);
    Y[0][0] = 10; Y[0][1] = 11; Y[0][2] = 12;
    Y[1][0] = 11; Y[1][1] = 13; Y[1][2] = 14;
    Y[2][0] = 12; Y[2][1] = 14; Y[2][2] = 15;

    double* V = vector(double, 3);
    V[0] = 16; V[1] = 17;  V[2] = 18;

    const network_state ns = (network_state) { 3, A, Y, V };

    int nss_count;

    network_state* nss = connected_components(ns, &nss_count);

    assert(nss_count == 1, -1, "Wrong number of connected components identified");
    char* error = "The # of nodes in the CC (%d) is different from the expected 3";
    assert(nss[0].size == 3, -1, error, nss[0]);

    network_state* lns = largest_component(nss, nss_count);

    assert(lns == &(nss[0]), -1, "The largest connected component does not point correctly");
}

void test_disjoint_network()
{
    bool** A = matrix(bool, 6, 6);
    A[0][0] = 0; A[0][1] = 1; A[0][2] = 0; A[0][3] = 1; A[0][4] = 0; A[0][5] = 0;
    A[1][0] = 1; A[1][1] = 0; A[1][2] = 0; A[1][3] = 0; A[1][4] = 0; A[1][5] = 0;
    A[2][0] = 0; A[2][1] = 0; A[2][2] = 0; A[2][3] = 0; A[2][4] = 0; A[2][5] = 0;
    A[3][0] = 1; A[3][1] = 0; A[3][2] = 0; A[3][3] = 0; A[3][4] = 0; A[3][5] = 0;
    A[4][0] = 0; A[4][1] = 0; A[4][2] = 0; A[4][3] = 0; A[4][4] = 0; A[4][5] = 1;
    A[5][0] = 0; A[5][1] = 0; A[5][2] = 0; A[5][3] = 0; A[5][4] = 1; A[5][5] = 0;

    double** Y = matrix(double, 6, 6);
    Y[0][0] = 21; Y[0][1] = 22; Y[0][2] = 23; Y[0][3] = 24; Y[0][4] = 25; Y[0][5] = 26;
    Y[1][0] = 22; Y[1][1] = 27; Y[1][2] = 28; Y[1][3] = 29; Y[1][4] = 30; Y[1][5] = 31;
    Y[2][0] = 23; Y[2][1] = 28; Y[2][2] = 32; Y[2][3] = 33; Y[2][4] = 34; Y[2][5] = 35;
    Y[3][0] = 24; Y[3][1] = 29; Y[3][2] = 33; Y[3][3] = 36; Y[3][4] = 37; Y[3][5] = 38;
    Y[4][0] = 25; Y[4][1] = 30; Y[4][2] = 34; Y[4][3] = 37; Y[4][4] = 39; Y[4][5] = 40;
    Y[5][0] = 26; Y[5][1] = 31; Y[5][2] = 35; Y[5][3] = 38; Y[5][4] = 40; Y[5][5] = 41;

    double* V = vector(double, 6);
    V[0] = 42; V[1] = 43;  V[2] = 44; V[3] = 45; V[4] = 46;  V[5] = 47;

    network_state ns = (network_state) { 6, A, Y, V };

    int nss_count;

    network_state* nss = connected_components(ns, &nss_count);

    assert(nss_count == 3, -1, "Wrong number of connected components identified");

    char* error = "The # of nodes in the CC (%d) is different from the expected 3";
    assert(nss[0].size == 3, -1, error, nss[0].size);
    assert(nss[1].size == 1, -1, error, nss[1].size);
    assert(nss[2].size == 2, -1, error, nss[2].size);

    error = "Error in the matrix sub-division for the first CC";

    assert(nss[0].A[0][0] == 0 && nss[0].Y[0][0] == 21, -1, error);
    assert(nss[0].A[0][1] == 1 && nss[0].Y[0][1] == 22, -1, error);
    assert(nss[0].A[0][2] == 1 && nss[0].Y[0][2] == 24, -1, error);
    assert(nss[0].A[0][1] == nss[0].A[1][0] && nss[0].Y[0][1] == nss[0].Y[1][0], -1, error);
    assert(nss[0].A[0][2] == nss[0].A[2][0] && nss[0].Y[0][2] == nss[0].Y[2][0], -1, error);
    assert(nss[0].A[1][1] == 0 && nss[0].Y[1][1] == 27, -1, error);
    assert(nss[0].A[1][2] == 0 && nss[0].Y[1][2] == 29, -1, error);
    assert(nss[0].A[1][2] == nss[0].A[2][1] && nss[0].Y[1][2] == nss[0].Y[2][1], -1, error);
    assert(nss[0].A[2][2] == 0 && nss[0].Y[2][2] == 36, -1, error);

    error = "Error in the matrix sub-division for the second CC";
    
    assert(nss[1].A[0][0] == 0 && nss[1].Y[0][0] == 32, -1, error);

    error = "Error in the matrix sub-division for the third CC";

    assert(nss[2].A[0][0] == 0 && nss[2].Y[0][0] == 39, -1, error);
    assert(nss[2].A[0][1] == 1 && nss[2].Y[0][1] == 40, -1, error);
    assert(nss[2].A[1][0] == 1 && nss[2].Y[1][0] == 40, -1, error);
    assert(nss[2].A[1][1] == 0 && nss[2].Y[1][1] == 41, -1, error);

    network_state* lns = largest_component(nss, nss_count);

    assert(lns == &(nss[0]), -1, "The largest connected component does not point correctly");
}

int util_components()
{
    test_fully_connected_network();
    test_disjoint_network();

    return 0;
}
