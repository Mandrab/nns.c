#include <float.h>
#include <gsl/gsl_rng.h>
#include <stdbool.h>

#include "util/distributions.h"
#include "util/errors.h"

#define SAMPLES_COUNT 1000000
#define QUANTS_COUNT 100
#define ACCURACY 0.005          // 0.5 % accuracy
#define SEED 42

void test_random_distribution_mean()
{
    // set the rng and its seed for the device generation
    gsl_rng* rng = gsl_rng_alloc(gsl_rng_random64_bsd);
    gsl_rng_set(rng, SEED);

    double accumulator = 0;

    // sum the generated values as to verify that the mean of the distribution is 0
    for (int i = 0; i < SAMPLES_COUNT; i++)
    {
        accumulator += normal_random(rng, 0.0f, 1.0f);
    }

    // get the average value of the array
    accumulator /= SAMPLES_COUNT;

    // check that the average is the mean of the distribution
    assert(
        -ACCURACY <= accumulator && accumulator <= ACCURACY, -1,
        "The mean of the distribution is %f instead of 0", accumulator
    );

    // reset the accumulator
    accumulator = 0;

    // sum the generated values as to verify that the mean of the distribution is 100
    for (int i = 0; i < SAMPLES_COUNT; i++)
    {
        accumulator += normal_random(rng, 100.0f, 1.0f);
    }

    // get the average value of the array
    accumulator /= SAMPLES_COUNT;

    // check that the average is the mean of the distribution
    assert(
        100 * (1 - ACCURACY) <= accumulator && accumulator <= 100 * (1 + ACCURACY),
        -1, "The mean of the distribution is %f instead of 100", accumulator
    );

    // free the random number generator
    gsl_rng_free(rng);
}

void test_random_distribution_shape()
{
    // set the rng and its seed for the device generation
    gsl_rng* rng = gsl_rng_alloc(gsl_rng_random64_bsd);
    gsl_rng_set(rng, SEED);

    // count how many values lie in various scaling of the std-dev
    // the expected is 68%, 95%, and 99.7% for each scale
    int count_sigma_1 = 0, count_sigma_2 = 0, count_sigma_3 = 0;

    // fill the samples array with number generated by the function
    for (int i = 0; i < SAMPLES_COUNT; i++)
    {
        double v = normal_random(rng, 0.0f, 1.0f);

        if (-1.0 < v && v < 1.0)
        {
            count_sigma_1++;
        }
        if (-2.0 < v && v < 2.0)
        {
            count_sigma_2++;
        }
        if (-3.0 < v && v < 3.0)
        {
            count_sigma_3++;
        }
    }

    double ratio = 100.0 * count_sigma_1 / SAMPLES_COUNT;
    bool test = 68 - 100 * ACCURACY <= ratio && ratio <= 68 + 100 * ACCURACY;
    assert(
        test, -1, "%f&& of samples instead of ~68%% are in [-1σ, 1σ]", ratio
    );

    ratio = 100.0 * count_sigma_2 / SAMPLES_COUNT;
    test = 95 - 100 * ACCURACY <= ratio && ratio <= 95 + 100 * ACCURACY;
    assert(
        test, -1, "%f&& of samples instead of ~95%% are in [-2σ, 2σ]", ratio
    );

    ratio = 100.0 * count_sigma_3 / SAMPLES_COUNT;
    test = 99.7 - 100 * ACCURACY <= ratio && ratio <= 99.7 + 100 * ACCURACY;
    assert(
        test, -1, "%f&& of samples instead of ~99.7%% are in [-2σ, 2σ]", ratio
    );

    // free the random number generator
    gsl_rng_free(rng);
}

int util_distributions()
{
    test_random_distribution_mean();
    test_random_distribution_shape();

    return 0;
}
