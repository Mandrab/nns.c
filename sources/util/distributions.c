#include <math.h>
#include <stdlib.h>

#include "util/distributions.h"

double normal_random(double µ, double σ) {
    double u1 = rand() / (double) RAND_MAX;
    double u2 = rand() / (double) RAND_MAX;
    double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    return µ + σ * z;
}
