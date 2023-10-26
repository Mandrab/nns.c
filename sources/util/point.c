#include <math.h>

#include "util/point.h"

double squared_distance(const point a, const point b)
{
    return pow(a.x - b.x, 2) + pow(a.y - b.y, 2);
}
