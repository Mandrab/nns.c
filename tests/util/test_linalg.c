#include <check.h>

#include "test_linalg.h"
#include "util/linalg.h"
#include "util/tensors.h"

START_TEST(test_conjugate_gradient)
{
    int size = 4;

    float** A = matrix(float, size, size);
    float x[size] = { };
    float b[size];

    // Ax = b:
    //
    //     -------------------------
    //     |       b + 3c      | a | = 0
    //     |  a                | b | = 1
    //     | 3a           + 4d | c | = 2
    //     |           4c      | d | = 1
    //     -------------------------
    A[0][0] = 0; A[0][1] = 1; A[0][2] = 3; A[0][3] = 0;
    A[1][0] = 1; A[1][1] = 0; A[1][2] = 0; A[1][3] = 0;
    A[2][0] = 3; A[2][1] = 0; A[2][2] = 0; A[2][3] = 4;
    A[3][0] = 0; A[3][1] = 0; A[3][2] = 4; A[3][3] = 0;

    b[0] = 0; b[1] = 1; b[2] = 2; b[3] = 1;

    // call the conjugate_gradient function
    conjugate_gradient(A, x, b, size);

    ck_assert_float_eq_tol(x[0],  1.00, 1e-6); // a
    ck_assert_float_eq_tol(x[1], -0.75, 1e-6); // b
    ck_assert_float_eq_tol(x[2],  0.25, 1e-6); // c
    ck_assert_float_eq_tol(x[3], -0.25, 1e-6); // d

    free_matrix(A, size);
}
END_TEST

Suite* linalg_suite()
{
    Suite* suite = suite_create("Linear algebra");
    TCase* tc_core = tcase_create("System of equation solution");

    tcase_add_test(tc_core, test_conjugate_gradient);
    suite_add_tcase(suite, tc_core);

    return suite;
}
