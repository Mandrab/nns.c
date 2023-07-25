#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <stdlib.h>

/**
 * Macro to perform condition-assertion and exit
 * with the specific error-code in case of error.
 * The macro expects a boolean condition as first
 * input, a possible error code as the second and
 * a message with possible parameters later.
 */
#define assert(EVAL, ERROR_CODE, ...)       \
    ({                                      \
        if (! (EVAL))                       \
        {                                   \
            printf(__VA_ARGS__);            \
            exit(ERROR_CODE);               \
        }                                   \
    })

#endif /* ERRORS_H */
