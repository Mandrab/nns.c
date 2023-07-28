#ifndef DISTRIBUTIONS_H
#define DISTRIBUTIONS_H

/// @brief Return a double value from a normal (a.k.a. gaussian) distribution
/// with the specified mu and sigma.
/// 
/// @param µ The expected median value of the distribution.
/// @param σ The sigma of the distribution representing how much wide it is. 
/// @return A double generated according to the gaussian distribution.
double normal_random(double µ, double σ);

#endif /* DISTRIBUTIONS_H */
