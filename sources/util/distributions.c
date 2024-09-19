#include <gsl/gsl_randist.h>

#include "util/distributions.h"

double normal_random(gsl_rng* rng, double µ, double σ)
{
    return µ + gsl_ran_gaussian(rng, σ);
}
