#include <check.h>
#include <stdlib.h>

#include "util/test_distributions.h"

int main()
{
    int number_failed;

    Suite* s = distributions_suite();
    SRunner* sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);

    srunner_free(sr);

    return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
