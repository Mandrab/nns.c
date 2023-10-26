#ifndef POINT_H
#define POINT_H

/// @brief Structure representing a point in the cartesian space.
typedef struct
{
    double x;   ///< The position in the first dimension of the point.
    double y;   ///< The position in the second dimension of the point.
} point;

/// @brief Compute the point to point distance with the euclidean rule without
/// performing the root. E.g.: (x1 - x2)^2 + (y1 - y2)^2
/// 
/// @param [in] a The first point.
/// @param [in] b The second point.
/// @return The distance between 'a' and 'b'.
double squared_distance(const point a, const point b);

#endif /* POINT_H */
