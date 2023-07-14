#include <math.h>
#include <stdlib.h>

#include "util/distributions.h"

float normal_random(float mu, float sigma) {
    float u1 = rand() / (float) RAND_MAX;
    float u2 = rand() / (float) RAND_MAX;
    float z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    return mu + sigma * z;
}
