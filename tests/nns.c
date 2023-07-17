#include <check.h>
#include <stdlib.h>

#include "util/test_components.h"
#include "util/test_distributions.h"
#include "util/test_linalg.h"

int main()
{
    int number_failed;

    Suite* s;
    SRunner* sr = srunner_create(s);

    srunner_add_suite(sr, (Suite*)distributions_suite());
    srunner_add_suite(sr, (Suite*)components_suite());
    srunner_add_suite(sr, (Suite*)linalg_suite());

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);

    srunner_free(sr);

    return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
