#ifndef DISTRIBUTIONS_H
#define DISTRIBUTIONS_H

#include <gsl/gsl_rng.h>

/// @brief Return a double value from a normal (a.k.a. gaussian) distribution
/// with the specified mu and sigma.
/// 
/// @param[in] rng The core random number generator to use to produce the
/// normal distribution.
/// @param[in] µ The expected median value of the distribution.
/// @param[in] σ The sigma of the distribution representing how wide it is.
/// @return A double generated according to the gaussian distribution.
double normal_random(gsl_rng* rng, double µ, double σ);

#endif /* DISTRIBUTIONS_H */
